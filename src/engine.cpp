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

#include <wiringPi.h>
#include <mosquitto.h>
#include <csignal>
#include <syslog.h>

#include <iostream>
#include <thread>
using namespace std;

#include <SQLiteCpp/Database.h>

#include "constants.hpp"
#include "utilities/databaseutils.hpp"




namespace hgardenpi
{
    inline namespace v1
    {

        const string check = "OK";

        //enable loop
        static volatile bool run = true;

        //exit signal handler
        static __sighandler_t handleSignal = [](int)
        {
            run = false;
        };

        Engine::Engine() : factory(new (nothrow) FactoryConcrete)
        {
            //initialize factory and all sub factory
            if (!factory) {
                throw runtime_error("no memory for factory");
            }
        }

        Engine::~Engine() noexcept
        {
            if (factory)
            {
                delete factory;
            }

            if (mqttClient)
            {
                delete mqttClient;
            }
            if (threadPool)
            {
                delete threadPool;
            }
        }

        void initialize()
        {
            //get pointers of system and device
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());

            //init system
            system->initialize();

            //init device
            device->setLogService(system->getLogService());
            device->initialize();

            //initialize threadPool and all sub factory
            Engine::getInstance()->threadPool = new (nothrow) ThreadPool(device->getInfo()->cpu);
            if (!Engine::getInstance()->threadPool) {
                throw runtime_error("no memory for threadPool");
            }
            device->setThreadPool(Engine::getInstance()->threadPool);

            //get database file path from config file
            string &dbFile = system->getConfigInfo()->database.file;

            //write sw version in log
            system->getLogService()->write(LOG_INFO, "database: %s", dbFile.c_str());

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
                auto &&version = DBGetVersion(database);
                if (version == 0)
                {
                    throw runtime_error("database initialized but version not found");
                }
                DBUpdate(database, version);
            }

            // //initialize mosquittopp
            // if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
            // {
            //     HGARDENPI_ERROR_LOG_AMD_THROW("mosquitto_lib_init() error")
            // }

            // //Globals::getInstance()->mqttClient = make_shared<MQTTClientMosquitto>(Globals::getInstance()->deviceInfo->serial, HGARDENPI_MQTT_BROKER_HOST, HGARDENPI_MQTT_BROKER_USER, HGARDENPI_MQTT_BROKER_PASSWD, HGARDENPI_MQTT_BROKER_PORT);
        }

        void start()
        {
            //get pointers of system and device
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());

            signal(SIGINT, handleSignal);
            signal(SIGTERM, handleSignal);

            system->start(run);
            device->start(run);

//            int i = 0;
            while (run)
            {
                //cout << "tick" << endl;
                //Globals::getInstance()->mqttClient->loop(run);
                // while (run)
                // {
                //     this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::TICK)));
                // }
                this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::TICK)));
//                if (i < 50)
//                {
//                    i++;
//                } else {
//                    run = false;
//                }

            }
        }
    }
}