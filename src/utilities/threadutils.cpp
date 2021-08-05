// MIT License
//
// Copyright (c) 2021. Happy GardenPI
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
//

//
// Created by Antonio Salsi on 29/07/21.
//

#include "threadutils.hpp"

#include <thread>
using namespace std;

namespace hgardenpi
{
    inline namespace v1
    {

        static constexpr const inline auto tick = static_cast<size_t>(Time::TICK);

        void threadSleep(volatile bool &run, mutex &m, size_t millis) noexcept
        {
            lock_guard<mutex> lg(m);
            printf("%d %d %d\n", tick, millis, millis / tick);

            size_t count = 0;
            while (run)
            {
                if (count >= millis / tick)
                {
                    return;
                }
                this_thread::sleep_for(chrono::milliseconds(tick));
                count ++;
                printf("%d\n", count);
            }
        }

        void threadSleep(volatile bool &run, mutex &m, Time &&millis) noexcept //keep not inline
        {
            threadSleep(run, m, static_cast<size_t>(millis));
        }


        void threadSleep(volatile bool &run, mutex &m, const Time &millis) noexcept //keep not inline
        {
            threadSleep(run, m, static_cast<size_t>(millis));
        }
    }
}