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

#include "threadengine.hpp"

#include <thread>

using namespace std;

namespace hgardenpi
{
    inline namespace v1
    {

#pragma region staticVariables
        static constexpr const inline auto tick = static_cast<size_t>(Time::TICK);
#pragma endregion staticVariables

#pragma region ThreadPool

        ThreadPool::ThreadPool(size_t threads)
                : stop(false),
                  threads(threads)
        {
            for (size_t i = 0; i < threads; ++i)
                workers.emplace_back(
                        [this]
                        {
                            for (;;)
                            {
                                function<void()> task;

                                {
                                    unique_lock<mutex> lock(this->queueMutex);
                                    this->condition.wait(lock,
                                                         [this]
                                                         { return this->stop || !this->tasks.empty(); });
                                    if (this->stop && this->tasks.empty())
                                        return;
                                    task = move(this->tasks.front());
                                    this->tasks.pop();
                                }

                                task();
                            }
                        }
                );
        }

        ThreadPool::~ThreadPool() noexcept
        {
            {
                unique_lock<mutex> lock(queueMutex);
                stop = true;
            }
            condition.notify_all();
            for (thread &worker: workers)
                worker.join();
        }

        ThreadPool *threadPool = nullptr;
#pragma endregion ThreadPool

#pragma region variables
        sigset_t sigset;
        atomic_bool shutdownRequest(false);
        mutex cvMutex;
        condition_variable cv;

        //exit signal handler
        function<int()> threadSignalHandler = []
        {

            int signum = 0;
            // wait until a signal is delivered:
            sigwait(&sigset, &signum);
            shutdownRequest.store(true);

            cout << "signum:" << to_string(signum) << endl;

            if (threadPool)
            {
                cout << "delete threadPool" << endl;
                delete threadPool;
                threadPool = nullptr;
            }
            // notify all waiting workers to check their predicate:
            cv.notify_all();
            return signum;
        };
#pragma endregion variables

#pragma region functions

        void threadSleep(volatile bool &run, mutex &m, Time &&millis) noexcept //keep not inline
        {
            this_thread::sleep_for(chrono::milliseconds(static_cast<size_t>(millis)));
        }

        void threadSleep(volatile bool &run, mutex &m, const Time &millis) noexcept //keep not inline
        {
            this_thread::sleep_for(chrono::milliseconds(static_cast<size_t>(millis)));
        }

#pragma endregion functions
    }
}