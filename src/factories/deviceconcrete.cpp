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

#include "deviceconcrete.hpp"

#include <wiringPi.h>

#include <stdexcept>
using std::runtime_error;

#include "../services/deviceservice.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        /**
         * @brief Before all call this functiuon fo inilialize the project
         * 
         * @exception runtime_error when hardware requisites mismatch
         */
        void DeviceConcrete::initialize()
        {
            //get device info
            deviceInfo = ::hgardenpi::v1::getDeviceInfo();

            //print device info to log
            logService->write(LOG_INFO, "hardware: %s", deviceInfo->hardware.c_str());
            logService->write(LOG_INFO, "revision: %s", deviceInfo->revision.c_str());
            logService->write(LOG_INFO, "serial: %s", deviceInfo->serial.c_str());
            logService->write(LOG_INFO, "model: %s", deviceInfo->model.c_str());
            logService->write(LOG_INFO, "cpu: %d", deviceInfo->cpu);

            //HW check
            if (deviceInfo->hardware != HW_V1 && deviceInfo->hardware != HW_V1_DEBUG)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("hardware not supported, you need a Raspberry Pi Zero W")
            }

            //initialize WiringPI
            wiringPiSetupGpio();
        }

        /**
         * @brief Start main look and scheduler
         * 
         */
        void DeviceConcrete::start()
        {
        }

        /**
         * @brief release a resorce
         * 
         */
        void DeviceConcrete::release() noexcept
        {
        }

        float DeviceConcrete::getCPUTemperature() const
        {
            return ::hgardenpi::v1::getCPUTemperature();
        }

        DeviceInfo::Ptr DeviceConcrete::getInfo() const
        {
            return deviceInfo;
        }

        string DeviceConcrete::getWlan0IP() const
        {
            return ::hgardenpi::v1::getWlan0IP();
        }

        string DeviceConcrete::getWlan0MAC() const
        {
            return ::hgardenpi::v1::getWlan0MAC();
        }

    }
}