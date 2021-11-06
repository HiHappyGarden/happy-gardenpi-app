// MIT License
//
// Copyright (c) 2021-2021. Happy GardenPI
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

#pragma once

#include <mutex>
#include <functional>
#include <queue>

#include "interfaces/initializable.hpp"
#include "interfaces/startable.hpp"
#include "interfaces/stoppable.hpp"
#include "interfaces/object.hpp"
#include "pods/aggregation.hpp"
#include "threadengine.hpp"
#include "constants.hpp"



namespace hgardenpi
{
    inline namespace v1
    {
        using namespace std;

        /**
         * @brief Scheduler for scheduling action, inside of this run a loop every one second
         */
        class Scheduler final : public Initializable, public Startable, public Stoppable, public Object
        {
        public:
            /**
             * @brief Prototype of callback on schedule start event
             */
            using OnScheduleStart = std::function<void(const Station::Ptr &)>;

            /**
             * @brief Prototype of callback on schedule end event
             */
            using OnScheduleEnd = std::function<void(const Station::Ptr &)>;

        private:
            mutable mutex m;

            ThreadPool *threadPool = nullptr;
            future<void> loopThread;

            OnScheduleStart onScheduleStart;
            OnScheduleEnd onScheduleEnd;

            Aggregations aggregations{};
            queue<Aggregation::Ptr> scheduledAggregations;
            queue<Station::Ptr> scheduledStations;

            atomic_bool inExecution;

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
            explicit Scheduler(ThreadPool *threadPool);
            ~Scheduler() override;

            /**
             * @brief Initialize internal data
             */
            void initialize() override;

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
             * @brief Insert into scheduler an aggregation of station
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            void schedule(Aggregation::Ptr &ptr);

            /**
             * @brief Shot one time an station triggered from user
             * @param ptr of station
             * @throw runtime_error when there some problem in station
             */
            void shot(const Station::Ptr &ptr);

            /**
             * @brief Set callback on start scheduledStations event
             * @param onScheduleStart on start event
             */
            inline void setScheduleStart(OnScheduleStart onScheduleStart) noexcept
            {
                this->onScheduleStart = move(onScheduleStart);
            }

            /**
             * @brief Set callback on end scheduledStations event
             * @param onScheduleEnd on start event
             */
            inline void setScheduleEnd(OnScheduleEnd onScheduleEnd) noexcept
            {
                this->onScheduleEnd = move(onScheduleEnd);
            }

            /**
             * Check a schedule is in execution
             * @return reference to inExecution, true if it's in execution
             */
            [[maybe_unused]] inline const atomic_bool &isInExecution() const noexcept
            {
                return inExecution;
            }

            /**
             * @brief Return the name of object
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }

        };

    }
}