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

namespace hgardenpi
{
    inline namespace v1
    {

        static SchedulerConcrete *self = nullptr;

        static bool loopRun = false;

        /**
         * @brief Check if execute a station in aggretation
         * @param now date/time now
         * @param aggregation aggregation to check
         */
        static void check(const tm* now, const Aggregation::Ptr aggregation);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
        void run()
        {
            self->m.lock();
            Aggregations aggregations(self->aggregations);
            self->m.unlock();
            while (wiringPiRunningThread && loopRun)
            {

                time_t t = time(0);   // get time now
                tm* now = localtime(&t);

                cout << "now wday:" << to_string(now->tm_wday) << " now hours:" << to_string(now->tm_hour) << " minutes:" << to_string(now->tm_min) << " second:" << to_string(now->tm_sec) << endl;

                for (auto &&aggregation : aggregations)
                {
                    lock_guard<mutex>(self->m);

                    auto &schedule = aggregation->schedule;

                    if (now->tm_hour == schedule.hour && now->tm_min == schedule.minute)
                    {
                        if (schedule.days & MONDAY && now->tm_wday == 1)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & TUESDAY && now->tm_wday == 2)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & WEDNESDAY && now->tm_wday == 3)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & THURSDAY && now->tm_wday == 4)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & FRIDAY && now->tm_wday == 5)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & SATURDAY && now->tm_wday == 6)
                        {
                            check(now, aggregation);
                        }
                        else if (schedule.days & SUNDAY && now->tm_wday == 7)
                        {
                            check(now, aggregation);
                        }
                    }
                }

                threadSleep(Time::SCHEDULER_TICK, loopRun);

            }
        }
#pragma clang diagnostic pop


        static void check(const tm* now, const Aggregation::Ptr aggregation)
        {
            for(auto &&station : aggregation->stations)
            {

            }
        }

        SchedulerConcrete::SchedulerConcrete(ThreadPool *threadPool) : threadPool(threadPool)
        {
            self = this;
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
            aggregations.clear();
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


