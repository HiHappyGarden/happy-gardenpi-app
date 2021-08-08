// MIT License
//
// Copyright (c) 2021 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "engine.hpp"

#include <mosquitto.h>
#include <syslog.h>

#include <csignal>
#include <iostream>
#include <SQLiteCpp/Database.h>

#include "constants.hpp"
#include "utilities/databaseutils.hpp"
#include "threadengine.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        //enable loops
        static volatile bool run = true;

        //exit signal handler
//        static inline const __sighandler_t handleSignal = [](int)
//        {
//            run = false;
//        };

        Engine::Engine() : factory(new (nothrow) FactoryConcrete)
        {
            //initialize factory and all sub factory
            if (!factory) {
                throw runtime_error("no memory for factory");
            }
        }

        Engine::~Engine() noexcept
        {
            //is not a mistake leave here
            if (threadPool)
            {
                cout << "delete threadPool" << endl;
                delete threadPool;
                threadPool = nullptr;
            }

            if (factory)
            {
                cout << "delete factory" << endl;
                delete factory;
                factory = nullptr;
            }

            if (mqttClient)
            {
                cout << "delete mqttClient" << endl;
                delete mqttClient;
                mqttClient = nullptr;
            }
            if (aggregationDao)
            {
                cout << "delete aggregationDao" << endl;
                delete aggregationDao;
                aggregationDao = nullptr;
            }
            if (stationDao)
            {
                cout << "delete stationDao" << endl;
                delete stationDao;
                stationDao = nullptr;
            }


        }

        void initialize()
        {
            //get pointers of system and device
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());

//            //init system
            system->initialize();

//            //init device
            device->setLogService(system->getLogService());
            device->initialize();

            //todo: to debug
//            //initialize threadPool in all sub factory
//            threadPool = new (nothrow) ThreadPool(device->getInfo()->cpu);
//            if (!threadPool) {
//                throw runtime_error(_("no memory for threadPool"));
//            }
            device->setThreadPool(threadPool);
            system->setThreadPool(threadPool);

            //get database file path from config file
            string &dbFile = system->getConfigInfo()->database.file;

            //write sw version in log
            system->getLogService()->write(LOG_INFO, "database: %s", dbFile.c_str());

            {
                //init database, out of SOLID pattern :)
                Database database(dbFile, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

                //check if database exist or not
                if (!database.tableExists(DB_METADATA_TABLE))
                {
                    DBUpdate(database);
                }
                else
                {
                    //update db structure to new one version
                    auto &&[version, wifiConfigured] = DBGetMetadata(database);
                    if (version == 0)
                    {
                        throw runtime_error(_("database initialized but version not found"));
                    }
                    DBUpdate(database, version);

                    Engine::getInstance()->wifiConfigured = wifiConfigured;
                }
            }


            Engine::getInstance()->aggregationDao = new (nothrow) AggregationDAO(dbFile);
            if(!Engine::getInstance()->aggregationDao)
            {
                throw runtime_error(_("no memory for aggregationDao"));
            }

            Engine::getInstance()->stationDao = new (nothrow) StationDAO(dbFile);
            if(!Engine::getInstance()->stationDao)
            {
                throw runtime_error(_("no memory for stationDao"));
            }


            // //initialize mosquittopp
            // //init database, out of SOLID pattern :)
            // if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
            // {
            //     HGARDENPI_ERROR_LOG_AMD_THROW("mosquitto_lib_init() error")
            // }

            // //Globals::getInstance()->mqttClient = make_shared<MQTTClientMosquitto>(Globals::getInstance()->deviceInfo->serial, HGARDENPI_MQTT_BROKER_HOST, HGARDENPI_MQTT_BROKER_USER, HGARDENPI_MQTT_BROKER_PASSWD, HGARDENPI_MQTT_BROKER_PORT);
        }

        void start()
        {
//            //get pointers of system and device
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());
            auto aggregationDao = Engine::getInstance()->aggregationDao;
            auto stationDao = Engine::getInstance()->stationDao;

//            //retrieve all active aggregations
            auto && aggregations = aggregationDao->getList();
            for (auto &&aggregation : aggregations)
            {
                aggregation->stations = move(stationDao->getList(aggregation));
            }

            //todo: to debug
//            device->setButtonOnClick([] {
//
//                printf("click\n");
//
//            });

//            //start all
            device->start(run);

//            //write stat service on log
            system->getLogService()->write(LOG_INFO, _("service ready"));

            //set signal behavior on SIGINT SIGTERM
            sigemptyset(&sigset);
            sigaddset(&sigset, SIGINT);
            sigaddset(&sigset, SIGTERM);
            pthread_sigmask(SIG_BLOCK, &sigset, nullptr);
            
            auto signalHandler = async(launch::async, threadSignalHandler);

            int signal = signalHandler.get();
            cout << "received signal " << signal << endl;
        }
    }
}