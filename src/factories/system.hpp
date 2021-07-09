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

#include "../services/configservice.hpp"
#include "../services/lockservice.hpp"
#include "../services/logservice.hpp"
#include "../services/deviceservice.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        /**
         * @brief Factory for management of system services devices
         * 
         */
        class System
        {

            /**
             * @brief Before all call this functiuon fo inilialize the project
             * 
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void initialize() = 0;

            /**
             * @brief Start main look and scheduler
             * 
             */
            virtual void start() = 0;

            /**
             * @brief Get the Config Info object
             * 
             * @return ConfigInfo::Ptr 
             */
            virtual ConfigInfo::Ptr getConfigInfo() const noexcept = 0;

            /**
             * @brief Get the Lock Service object
             * 
             * @return const LockService*
             */
            virtual const LockService *getLockService() const = 0;

            /**
             * @brief Get the Log Service object
             * 
             * @return const LogService& 
             */
            virtual const LogService *getLogService() const = 0;

            /**
             * @brief Get CPU temperature
             * @exception throw exception when temp is not available
             * @return return degrees temperature
             */
            virtual float getCPUTemperature() const = 0;

            /**
             * @brief Get Raspberry PI info
             * @exception throw exception when temp is not available
             * @return return Raspberry PI info
             */
            virtual DeviceInfo::Ptr getDeviceInfo() const = 0;
        };

    }
}
