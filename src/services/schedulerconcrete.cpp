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

namespace hgardenpi
{
    inline namespace v1
    {

        static SchedulerConcrete *self = nullptr;

        static bool loopRun = false;

        void check(const Aggregation::Ptr aggregation);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
        void run(Aggregations &ref)
        {
            self->m.lock();
            queue aggregations(ref);
            ref.clear();
            self->m.unlock();
            while (wiringPiRunningThread && loopRun)
            {

                //cycle aggregation for create a queue of stations to handle
                {
                    lock_guard<mutex>(self->m);

#if HGARDENPI_TEST > 0
                    //test
                    tm* now = new tm {
                        .tm_min = 30,
                        .tm_hour = 10,
                        .tm_wday = 5
                    };
                    cout << "now wday:" << to_string(now->tm_wday) << " now hours:" << to_string(now->tm_hour) << " minutes:" << to_string(now->tm_min) << endl;
#else
                    time_t t = time(0);   // get time now
                    tm* now = localtime(&t);
#endif

                    while (!ref.empty())
                    {
                        auto &&aggregation = ref.front();
                        auto &schedule = aggregation->schedule;

                        if (now->tm_hour == schedule.hour && now->tm_min == schedule.minute)
                        {
                            if (schedule.days & MONDAY && now->tm_wday == 1)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & TUESDAY && now->tm_wday == 2)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & WEDNESDAY && now->tm_wday == 3)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & THURSDAY && now->tm_wday == 4)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & FRIDAY && now->tm_wday == 5)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & SATURDAY && now->tm_wday == 6)
                            {
                                check(aggregation);
                            }
                            else if (schedule.days & SUNDAY && now->tm_wday == 7)
                            {
                                check(aggregation);
                            }
                        }

                        aggregations.pop();
                    }

#if HGARDENPI_TEST > 0
                    //test
                    delete now;
//                    self->aggregations.clear();
#endif

                    //cicle all scheduled event
                    {
                        unique_lock<mutex> ul(self->m);

                        while (self->scheduled.empty())
                        {
                            auto &&station = self->scheduled.front();

                            self->onScheduleStart(station);

                            threadSleep(station->wateringTime * 1'00);

                            self->onScheduleEnd();

                            self->scheduled.pop();
                        }
                    }

                }



                threadSleep(Time::SCHEDULER_TICK, loopRun);

            }
        }
#pragma clang diagnostic pop


        void check(const Aggregation::Ptr aggregation)
        {
            for(auto &&station : aggregation->stations)
            {
                self->scheduled.push(station);
                cout << "station:" << to_string(station->id) << endl;
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


        void SchedulerConcrete::start()
        {
            loopRun = true;
            loopThread =  move(threadPool->enqueue(&run, aggregations));

        }


        void SchedulerConcrete::stop()
        {
            lock_guard<mutex> lg(m);
            loopRun = false;
            aggregations.clear();

            scheduled = queue<Station::Ptr>();
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


        void SchedulerConcrete::shot(const Aggregation::Ptr &ptr) const
        {
            lock_guard<mutex> lg(m);
        }


        void SchedulerConcrete::shot(const Station::Ptr &ptr) const
        {
            lock_guard<mutex> lg(m);
        }

    }
}


