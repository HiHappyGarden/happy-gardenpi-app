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
// Created by Antonio Salsi on 24/07/21.
//

#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "../interfaces/object.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        using namespace std;

        /**
         * @brief Thread Pool class
         */
        class ThreadPool final : public Object
        {
        public:
            explicit ThreadPool(size_t);

            /**
             * @brief the destructor joins all threads
             */
            ~ThreadPool() noexcept override;

            HGARDENPI_NO_COPY_NO_MOVE(ThreadPool)

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
            -> future<typename result_of<F(Args...)>::type>;

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
            mutex queue_mutex;
            condition_variable condition;
            bool stop;
            size_t threads;
        };

    }
}

