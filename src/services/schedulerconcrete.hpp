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

#pragma once

#include <mutex>
#include <functional>

#include "../threadengine.hpp"
#include "../constants.hpp"
#include "scheduler.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        using std::mutex;
        using std::string;

        /**
         * @brief Scheduler for scheduling action, inside of this run a loop every one second
         */
        class SchedulerConcrete final : public Scheduler
        {
            mutable mutex m;

            const ThreadPool *threadPool = nullptr;

        public:
            explicit SchedulerConcrete(const ThreadPool *threadPool);
            ~SchedulerConcrete() override = default;

            HGARDENPI_NO_COPY_NO_MOVE(SchedulerConcrete)

            /**
             * @brief Start main look and scheduler
             * @exception runtime_error when hardware requisites mismatch
             */
            void start() override;

            /**
             * Insert into scheduler an aggregation of station
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            void schedule(const Aggregation::Ptr &ptr) override;

            /**
             * Shot one time an aggregation of station triggered from user
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            void shot(const Aggregation::Ptr &ptr) const override;

            /**
             * Shot one time an station triggered from user
             * @param ptr of station
             * @throw runtime_error when there some problem in station
             */
            void shot(const Station::Ptr &ptr) const override;

            /**
             * @brief Return the name of object
             * 
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }

        };

    }
}