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

#include "../threadengine.hpp"

using hgardenpi::v1::SchedulerConcrete;

SchedulerConcrete::SchedulerConcrete(ThreadPool *threadPool) : threadPool(threadPool)
{

}


void SchedulerConcrete::start()
{
    threadPool->enqueue([]
    {
        while (wiringPiRunningThread)
        {

            threadSleep(Time::SCHEDULER_TICK);
        }
    });
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

//void SchedulerConcrete::remove(const Aggregation::Ptr &ptr)
//{
//    lock_guard<mutex> lg(m);
//}


void SchedulerConcrete::shot(const Aggregation::Ptr &ptr) const
{
    lock_guard<mutex> lg(m);
}


void SchedulerConcrete::shot(const Station::Ptr &ptr) const
{
    lock_guard<mutex> lg(m);
}


