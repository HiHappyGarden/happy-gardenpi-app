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

#include "../services/configserviceconcrete.hpp"
#include "../services/lockservice.hpp"
#include "../services/logservice.hpp"
#include "../services/scheduler.hpp"
#include "../interfaces/startable.hpp"
#include "../interfaces/releasable.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        class ThreadPool;

        /**
         * @brief Factory for management of system services devices
         * 
         */
        class System : public Startable, public Releasable, public Object
        {

        public:
            using Startable::initialize;
            using Startable::start;

            ~System() override = default;

            /**
             * @brief Get the Config Info object
             * 
             * @return ConfigInfo::Ptr 
             */
            [[nodiscard]] virtual ConfigInfo::Ptr getConfigInfo() const noexcept = 0;

            /**
             * @brief Get the Lock Service object
             * 
             * @return const LockService *
             */
            [[nodiscard]] virtual const LockService *getLockService() const noexcept = 0;

            /**
             * @brief Get the Log Service object
             * 
             * @return const LogService *
             */
            [[nodiscard]] virtual const LogService *getLogService() const noexcept = 0;

            /**
             * @brief Get the Service Scheduler
             *
             * @return const Scheduler *
             */
            [[nodiscard]] virtual const Scheduler *getScheduler() const noexcept = 0;

            /**
             * Set ThreadPool instance
             * @param threadPool instance
             * @throw runtime_error when something goes wrong
             */
            virtual void setThreadPool(const ThreadPool *threadPool) = 0;
        };

    }
}
