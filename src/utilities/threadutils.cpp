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

        void threadSleep(volatile bool &run, size_t millis) noexcept
        {
            static const auto tick = static_cast<size_t>(Time::TICK) / 10;
            size_t count = 0;
            while (run)
            {
                if (count >= millis / tick)
                {
                    return;
                }
                this_thread::sleep_for(chrono::milliseconds(tick));
                count ++;
            }
        }

        void threadSleep(volatile bool &run, Time &&millis) noexcept //keep not inline
        {
            threadSleep(run, static_cast<size_t>(millis));
        }


        void threadSleep(volatile bool &run, const Time &millis) noexcept //keep not inline
        {
            threadSleep(run, static_cast<size_t>(millis));
        }
    }
}