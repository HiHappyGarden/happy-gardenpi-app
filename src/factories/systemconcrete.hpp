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

#include "system.hpp"

#include "../services/configserviceconcrete.hpp"
#include "../services/lockserviceconcrete.hpp"
#include "../services/logserviceconcrete.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        /**
         * @brief Factory for management of system services devices
         * 
         */
        class SystemConcrete final : public System
        {

            mutable ConfigInfo::Ptr configInfo = nullptr;
            mutable LockService *lockService = nullptr;
            mutable LogService *logService = nullptr;
            mutable Scheduler *scheduler = nullptr;
            ThreadPool *threadPool;

        public:
            SystemConcrete() = default;
            ~SystemConcrete() override;
            HGARDENPI_NO_COPY_NO_MOVE(SystemConcrete)

            /**
             * @brief Before all call this function fo initialize the project
             * 
             * @exception runtime_error when hardware requisites mismatch
             */
            void initialize() override;

            /**
             * @brief close all resource
             * 
             */
            void close() noexcept override;

            /**
             * @brief Get the Config Info object
             * 
             * @return ConfigInfo::Ptr 
             */
            inline ConfigInfo::Ptr getConfigInfo() const noexcept override
            {
                return configInfo;
            }

            /**
             * @brief Get the Lock Service object
             * 
             * @return const LockService*
             */
            inline const LockService *getLockService() const noexcept override
            {
                return lockService;
            }

            /**
             * @brief Get the Log Service object
             * 
             * @return const LogService& 
             */
            inline const LogService *getLogService() const noexcept override
            {
                return logService;
            }

            /**
             * @brief Get the Service Scheduler
             *
             * @return const Scheduler *
             */
            [[nodiscard]] Scheduler *getScheduler() const noexcept override
            {
                return scheduler;
            }

            /**
             * Initialize ThreadPool instance
             * @param threadNumber number of thread
             * @throw runtime_error when something goes wrong
             */
            void initializeThreadPool(size_t threadNumber) override;

            /**
             * @brief Initialize scheduler after initializeThreadPool
             * @throw runtime_error when something goes wrong
             */
            void initializeScheduler() override;

            [[nodiscard]] inline ThreadPool * getThreadPool() const noexcept override
            {
                return threadPool;
            }

            /**
             * Start system service
             */
            void start() override;

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
