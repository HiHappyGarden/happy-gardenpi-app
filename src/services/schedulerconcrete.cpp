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

#include "schedulerconcrete.hpp"

#include <iostream>

#include "../config.h"
#include "../threadengine.hpp"

using Schedule = hgardenpi::v1::Aggregation::Schedule;

namespace hgardenpi
{
    inline namespace v1
    {


        static SchedulerConcrete *self = nullptr;

        static bool loopRun = false;

        void check(const Aggregation::Ptr aggregation);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
        void run()
        {
            while (wiringPiRunningThread && loopRun)
            {

                //cycle aggregation for create a queue of stations to handle
                {
                    lock_guard<mutex>(self->m);

#if HGARDENPI_TEST > 0
                    //test
                    tm* now = new tm {
                        .tm_sec = 32,
                        .tm_min = 30,
                        .tm_hour = 10,
                        .tm_wday = 5,

                    };
                    cout << "now wday:" << to_string(now->tm_wday) << " now hours:" << to_string(now->tm_hour) << " minutes:" << to_string(now->tm_min) << " sec:" << to_string(now->tm_sec) << endl;
#else
                    time_t t = time(0);   // get time now
                    tm* now = localtime(&t);
#endif

                    while (!self->scheduledAggregations.empty())
                    {
                        auto &&aggregation = self->scheduledAggregations.front();
                        auto &schedule = aggregation->schedule;

                        //check if the aggregation is scheduled now fot hours and minutes, if NOT_SET enter every loop
                        //fill queue scheduledStations with the station to schedule
                        if ( (now->tm_hour == schedule.hour || schedule.hour == Schedule::NOT_SET) && (now->tm_min == schedule.minute || schedule.minute == Schedule::NOT_SET))
                        {
                            //check if aggregation is scheduled monday, if NOT_SET enter every loop
                            if (schedule.days & MONDAY && now->tm_wday == 1 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                            //check if aggregation is scheduled tuesday, if NOT_SET enter every loop
                            else if (schedule.days & TUESDAY && now->tm_wday == 2 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                            //check if aggregation is scheduled wednesday, if NOT_SET enter every loop
                            else if (schedule.days & WEDNESDAY && now->tm_wday == 3 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                            //check if aggregation is thursday wednesday, if NOT_SET enter every loop
                            else if (schedule.days & THURSDAY && now->tm_wday == 4 || schedule.days == Schedule::NOT_SET )
                            {
                                check(aggregation);
                            }
                            //check if aggregation is friday wednesday, if NOT_SET enter every loop
                            else if (schedule.days & FRIDAY && now->tm_wday == 5 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                            //check if aggregation is saturday wednesday, if NOT_SET enter every loop
                            else if (schedule.days & SATURDAY && now->tm_wday == 6 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                            //check if aggregation is sunday wednesday, if NOT_SET enter every loop
                            else if (schedule.days & SUNDAY && now->tm_wday == 7 || schedule.days == Schedule::NOT_SET)
                            {
                                check(aggregation);
                            }
                        }
                        self->scheduledAggregations.pop();
                    }

#if HGARDENPI_TEST > 0
                    //test
                    delete now;
#endif

                    //cycle all scheduledStations event for watering a station of garden
                    {
                        unique_lock<mutex> ul(self->m);

                        while (!self->scheduledStations.empty() && wiringPiRunningThread)
                        {
                            //set flag in execution
                            self->inExecution = true;

                            //get first station
                            auto &&station = self->scheduledStations.front();

                            // start event
                            self->onScheduleStart(station);

                            // sleep whit station's watering time
                            threadSleep(station->wateringTime * 5'00);

                            // end event
                            self->onScheduleEnd(station);

                            // remove from queue
                            self->scheduledStations.pop();

                            threadSleep(Time::SCHEDULER_NETX_TICK);

                            //set flag in execution
                            self->inExecution = false;
                        }
                    }

                }

                threadSleep(Time::SCHEDULER_TICK, loopRun);
            }
        }
#pragma clang diagnostic pop


        inline void check(const Aggregation::Ptr aggregation)
        {
            for(auto &&station : aggregation->stations)
            {
                self->scheduledStations.push(station);
            }
        }

        SchedulerConcrete::SchedulerConcrete(ThreadPool *threadPool) : threadPool(threadPool)
        {
            self = this;
        }

        SchedulerConcrete::~SchedulerConcrete()
        {
            if (loopRun)
            {
                stop();
            }
        }

        void SchedulerConcrete::initialize()
        {
            if (scheduledAggregations.empty())
            {
                self->m.lock();
                for (auto &&it : aggregations)
                {
                    scheduledAggregations.push(it);
                }
                self->m.unlock();
            }
        }

        void SchedulerConcrete::start()
        {
            loopRun = true;
            loopThread =  move(threadPool->enqueue(&run));
        }


        void SchedulerConcrete::stop()
        {
            lock_guard<mutex> lg(m);
            loopRun = false;

            scheduledStations = queue<Station::Ptr>();
            scheduledAggregations = queue<Aggregation::Ptr>();
        }


        void SchedulerConcrete::schedule(Aggregation::Ptr &ptr)
        {
            lock_guard<mutex> lg(m);
            for (auto &&it : aggregations)
            {
                if(it->id == ptr->id)
                {
                    aggregations.remove(it);
                    break;
                }
            }
            aggregations.push_back(ptr);
        }

        void SchedulerConcrete::shot(const Station::Ptr &ptr)
        {
            lock_guard<mutex> lg(m);
            scheduledStations.push(ptr);
        }

    }
}


