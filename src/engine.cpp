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
#include <hgardenpi-protocol/utilities/stringutils.hpp>
using namespace hgardenpi::protocol;


#include "constants.hpp"
#include "factories/factoryconcrete.hpp"
#include "utilities/databaseutils.hpp"

#include "components/relaymodule4channel.hpp"
#include "components/display.hpp"
#include "threadengine.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        static void onSchedulerEventStart(const Station::Ptr &station);

        static void onSchedulerEventEnd(const Station::Ptr &station);

        Engine::Engine()
        : factory(new (nothrow) FactoryConcrete)
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
            auto communication = const_cast<Communication *>(Engine::getInstance()->factory->getCommunication());

            //init system
            system->initialize();

            //init device
            device->setLogService(system->getLogService());
            device->initialize();

            //init communication
            communication->setInfos(device->getInfo()->serial, system->getConfigInfo());
            communication->setLogService(system->getLogService());
            communication->initialize();

            //initialize threadPool
            system->initializeThreadPool(device->getInfo()->cpu);

            //set threadPool to device
            device->setThreadPool(system->getThreadPool());

            //initialize scheduler here because I attempt threadPool is initialized
            system->initializeScheduler();

            //get database file path from config file
            const string &dbFile = system->getConfigInfo()->database.file;

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
                    const auto &&[version, wifiConfigured] = DBGetMetadata(database);
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

        }

        void start()
        {
            //get pointers of all element
            auto system = const_cast<System *>(Engine::getInstance()->factory->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->factory->getDevice());
            auto communication = const_cast<Communication *>(Engine::getInstance()->factory->getCommunication());
            auto aggregationDao = Engine::getInstance()->aggregationDao;
            auto stationDao = Engine::getInstance()->stationDao;

            //retrieve all active aggregations and set to scheduler
            auto &&aggregations = aggregationDao->getList();
            for (auto &&aggregation : aggregations)
            {
                aggregation->stations = move(stationDao->getList(aggregation));
                system->getScheduler()->schedule(aggregation);
            }
            system->getScheduler()->initialize();

            //set all callback
            system->getScheduler()->setScheduleStart(&onSchedulerEventStart);
            system->getScheduler()->setScheduleEnd(&onSchedulerEventEnd);

            //start all
            device->start(); //keep this position
            system->start(); //keep this position
            communication->start();

            //write stat service on log
            system->getLogService()->write(LOG_INFO, _("service ready"));

            //enable broker loop
            communication->loop();
        }

        static void onSchedulerEventStart(const Station::Ptr &station) try
        {
            auto system = const_cast<System *>(Engine::getInstance()->getFactory()->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->getFactory()->getDevice());

            //start water supply
            device->getRelayModule()->setRelay(station, true);

            //turn on contrast display
            device->getDisplay()->setContrastTurnOn(true);

            //turn off interna display info capability
            device->setPrintOnDisplayInternalInfo(false);

            //print info water supply
            device->printOnDisplay(_(("Water supply|" + station->name).c_str()), true);
            system->getLogService()->write(LOG_INFO,"Start station: %s", station->name.c_str());
        }
        catch (const exception &e)
        {
            const_cast<System *>(Engine::getInstance()->getFactory()->getSystem())->getLogService()->write(LOG_WARNING,"exception on start station: %s what: %s", station->name.c_str(), e.what());
        }


        static void onSchedulerEventEnd(const Station::Ptr &station) try
        {
            auto system = const_cast<System *>(Engine::getInstance()->getFactory()->getSystem());
            auto device = const_cast<Device *>(Engine::getInstance()->getFactory()->getDevice());

            //end water supply
            device->getRelayModule()->setRelay(station, false);

            //turn off contrast display
            device->getDisplay()->setContrastTurnOn(false);

            //turn on interna display info capability
            device->setPrintOnDisplayInternalInfo(true);

            //clear display
            device->getDisplay()->clear();

            //log end
            system->getLogService()->write(LOG_INFO, "End station: %s", station->name.c_str());
        }
        catch (const exception &e)
        {
            const_cast<System *>(Engine::getInstance()->getFactory()->getSystem())->getLogService()->write(LOG_WARNING,"exception on end station: %s what: %s", station->name.c_str(), e.what());
        }

    }
}