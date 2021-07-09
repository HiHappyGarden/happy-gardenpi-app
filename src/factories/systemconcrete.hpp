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

#include "../services/deviceservice.hpp"
#include "../services/configservice.hpp"
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

            mutable DeviceInfo::Ptr deviceInfo = nullptr;
            mutable ConfigInfo::Ptr configInfo = nullptr;
            mutable LockService *lockService = nullptr;
            mutable LogService *logService = nullptr;

        public:
            ~SystemConcrete();

            /**
             * @brief Before all call this functiuon fo inilialize the project
             * 
             * @exception runtime_error when hardware requisites mismatch
             */
            void initialize() override;

            /**
             * @brief Start main look and scheduler
             * 
             */
            void start() override;

            /**
             * @brief Get the Config Info object
             * 
             * @return ConfigInfo::Ptr 
             */
            ConfigInfo::Ptr getConfigInfo() const noexcept override;

            /**
             * @brief Get the Lock Service object
             * 
             * @return const LockService*
             */
            const LockService *getLockService() const override;

            /**
             * @brief Get the Log Service object
             * 
             * @return const LogService& 
             */
            const LogService *getLogService() const override;

            /**
             * @brief Get CPU temperature
             * @exception throw exception when temp is not available
             * @return return degrees temperature
             */
            float getCPUTemperature() const override;

            /**
             * @brief Get Raspberry PI info
             * @exception throw exception when temp is not available
             * @return return Raspberry PI info
             */
            DeviceInfo::Ptr getDeviceInfo() const override;
        };

    }
}
