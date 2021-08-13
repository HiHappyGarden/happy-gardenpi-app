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
#include <queue>

#include "scheduler.hpp"
#include "../threadengine.hpp"
#include "../constants.hpp"


namespace hgardenpi
{
    inline namespace v1
    {
        using namespace std;

        /**
         * @brief Scheduler for scheduling action, inside of this run a loop every one second
         */
        class SchedulerConcrete final : public Scheduler
        {
            mutable mutex m;

            ThreadPool *threadPool = nullptr;
            future<void> loopThread;

            OnScheduleStart onScheduleStart;
            OnScheduleEnd onScheduleEnd;

            Aggregations aggregations{};
            queue<Station::Ptr> scheduled;

            /**
             * @brief function executed in loop
             */
            friend void run();

            /**
             * @brief Check if execute a station in aggretation
             * @param aggregation aggregation to check
             */
            friend void check(const Aggregation::Ptr aggregation);

        public:

            /**
             * @brief Create a new instance
             */
            explicit SchedulerConcrete(ThreadPool *threadPool);
            ~SchedulerConcrete() override;

            HGARDENPI_NO_COPY_NO_MOVE(SchedulerConcrete)

            /**
             * @brief Start service
             * @exception runtime_error when something goes wrong
             */
            void start() override;

            /**
             * @brief Stop service
             * @exception runtime_error when something goes wrong
             */
            void stop() override;

            /**
             * Insert into scheduler an aggregation of station
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            void schedule(Aggregation::Ptr &ptr) override;

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

            /**
             * @brief Set callback on start scheduled event
             * @param onScheduleStart on start event
             */
            inline void setScheduleStart(OnScheduleStart onScheduleStart) noexcept override
            {
                this->onScheduleStart = move(onScheduleStart);
            }

            /**
             * @brief Set callback on end scheduled event
             * @param onScheduleEnd on start event
             */
            inline void setScheduleEnd(OnScheduleEnd onScheduleEnd) noexcept override
            {
                this->onScheduleEnd = move(onScheduleEnd);
            }

        };

    }
}