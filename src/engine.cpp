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

#include <syslog.h>

#include <SQLiteCpp/Database.h>

#include "constants.hpp"
#include "utilities/databaseutils.hpp"
#include "clients/mqttclientmosquitto.hpp"
#include "threadengine.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        static const LogService *logService = nullptr;

        /**
         * @brief Event triggered when MqttClient message arrive
         * @param data message data
         */
        static void onMqttClientMessageCallback(const uint8_t *data);

        static void onSchedulerEvent(const Aggregation::Ptr &aggregation, const Station::Ptr &station);

        Engine::Engine() : factory(new (nothrow) FactoryConcrete)
        {
            //initialize factory and all sub factory
            if (!factory) {
                throw runtime_error("no memory for factory");
            }
        }

        Engine::~Engine() noexcept
        {
            //stop WiringPI thread
            wiringPiRunningThread = false;

            if (factory)
            {
                delete factory;
                factory = nullptr;
            }

            if (mqttClient)
            {
                delete mqttClient;
                mqttClient = nullptr;
            }
            if (aggregationDao)
            {
                delete aggregationDao;
                aggregationDao = nullptr;
            }
            if (stationDao)
            {
                delete stationDao;
                stationDao = nullptr;
            }
        }

        void initialize()
        {
            //get pointers of system and device
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());

            //init system
            system->initialize();

            //reference a local poiter of systemService
            logService = system->getLogService();

            //init device
            device->setLogService(logService);
            device->initialize();

            //initialize threadPool
            system->initializeThreadPool(device->getInfo()->cpu);

            //set threadPool to device
            device->setThreadPool(system->getThreadPool());

            //initialize scheduler here because I attempt threadPool is initialized
            system->initializeScheduler();

            //get database file path from config file
            string &dbFile = system->getConfigInfo()->database.file;

            //write sw version in log
            logService->write(LOG_INFO, "database: %s", dbFile.c_str());

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
                        throw runtime_error("database initialized but version not found");
                    }
                    DBUpdate(database, version);

                    Engine::getInstance()->wifiConfigured = wifiConfigured;
                }
            }


            Engine::getInstance()->aggregationDao = new (nothrow) AggregationDAO(dbFile);
            if(!Engine::getInstance()->aggregationDao)
            {
                throw runtime_error("no memory for aggregationDao");
            }

            Engine::getInstance()->stationDao = new (nothrow) StationDAO(dbFile);
            if(!Engine::getInstance()->stationDao)
            {
                throw runtime_error("no memory for stationDao");
            }


            //initialize mosquittopp
            Engine::getInstance()->mqttClient = new (nothrow) MQTTClientMosquitto(device->getInfo()->serial,
                                                                        system->getConfigInfo()->broker.host,
                                                                        system->getConfigInfo()->broker.user,
                                                                        system->getConfigInfo()->broker.passwd,
                                                                        system->getConfigInfo()->broker.port
            );
            if(!Engine::getInstance()->mqttClient)
            {
                throw runtime_error("no memory for mqttClient");
            }
            Engine::getInstance()->mqttClient->setLogService(logService);
            Engine::getInstance()->mqttClient->initialize();

        }

        void start()
        {
            //get pointers of all element
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());
            auto mqttClient = Engine::getInstance()->mqttClient;
            auto aggregationDao = Engine::getInstance()->aggregationDao;
            auto stationDao = Engine::getInstance()->stationDao;

            //retrieve all active aggregations and set to scheduler
            auto && aggregations = aggregationDao->getList();
            for (auto &&aggregation : aggregations)
            {
                aggregation->stations = move(stationDao->getList(aggregation));
                system->getScheduler()->schedule(aggregation);
            }

            //set all callback
            device->setOnButtonClick([]
            {

                printf("click\n");

            });
            mqttClient->setOnMessageCallback(&onMqttClientMessageCallback);
            system->getScheduler()->setOnExecute(&onSchedulerEvent);

            //start all
            device->start(); //keep this position
            system->start(); //keep this position
            mqttClient->start();

            //write stat service on log
            logService->write(LOG_INFO, _("service ready"));

            //enable broker loop
            mqttClient->loop();

        }

        static void onMqttClientMessageCallback(const uint8_t *data)
        {
            cout << "msg:" << data << endl;
        }

        static void onSchedulerEvent(const Aggregation::Ptr &aggregation, const Station::Ptr &station)
        {
            cout << "event aggregation:" << aggregation->id  << "station:" << station->id << endl;
        }
    }
}