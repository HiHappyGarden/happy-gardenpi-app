// MIT License
//
// Copyright (c) $year. Happy GardenPI
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

#pragma once

#include <queue>
#include <future>

#include "interfaces/object.hpp"

//from wiringPi Happy GardenPi fork
extern volatile unsigned wiringPiRunningThread;

namespace hgardenpi
{
    inline namespace v1
    {

        using namespace std;

#pragma region ThreadPool

        /**
         * @brief Thread Pool class
         */
        class ThreadPool final : public Object
        {


        public:

            /**
             * @brief the constructor just launches some amount of workers
             */
            explicit ThreadPool(size_t);

            /**
             * @brief the destructor joins all threads
             */
            ~ThreadPool() noexcept override;

            /**
             * @brief add new work item to the pool
             * @tparam F function to execute
             * @tparam Args arguments
             * @param f concrete lambda
             * @param args concrete args
             * @return future result
             */
            template<class F, class... Args>
            auto enqueue(F &&f, Args &&... args)
            -> future<typename result_of<F(Args...)>::type>
            {
                using ReturnType = typename result_of<F(Args...)>::type;

                auto task = make_shared<packaged_task<ReturnType()> >(
                        bind(forward<F>(f), forward<Args>(args)...)
                );

                future<ReturnType> res = task->get_future();
                {
                    unique_lock<mutex> lock(queueMutex);

                    // don't allow enqueueing after stopping the pool
                    if (stop)
                        throw runtime_error("enqueue on stopped ThreadPool");

                    tasks.emplace([task]
                    {
                        (*task)();
                    });
                }
                condition.notify_one();

                return res;
            }

            /**
             * @brief Return the name of object
             *
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return string(typeid(*this).name()) + " threadPooled" + to_string(threads);
            }

        private:
            // need to keep track of threads so we can join them
            vector<thread> workers;

            // the task queue
            queue<function<void()>> tasks;

            // synchronization
            mutex queueMutex;
            condition_variable condition;
            bool stop;
            size_t threads;
        };

#pragma endregion ThreadPool

#pragma region variables
        extern long pidMain;

#pragma endregion variables

#pragma region functions

        /**
         * @brief Sleep a thread for n millis
         * @param millis to sleep
         * @param conditions add a external conditions for interrupt the loop
         */
        [[maybe_unused]] void threadSleep(uint64_t millis, bool conditions = true) noexcept;

        /**
         * @brief Sleep a thread by constant
         * @param millis to sleep
         * @param conditions add a external conditions for interrupt the loop, true enable loop
         */
        [[maybe_unused]] inline void threadSleep(const Time &millis, bool conditions = true) noexcept
        {
            threadSleep(static_cast<uint64_t>(millis), conditions);
        }

        /**
         * @brief Sleep a thread for by constant
         * @param millis to sleep
         * @param conditions add a external conditions for interrupt the loop, true enable loop
         */
        [[maybe_unused]] inline void threadSleep(Time &&millis, bool conditions = true) noexcept
        {
            threadSleep(static_cast<uint64_t>(millis), conditions);
        }

        /**
         * @brief Get the pid fot thread child
         * @param exclude exclude some pid
         */
        [[maybe_unused]] vector<long> threadGetChildPid(long exclude = -1);

#pragma endregion functions

    }
}


