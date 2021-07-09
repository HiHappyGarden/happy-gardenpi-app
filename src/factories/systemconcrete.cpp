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

#include "systemconcrete.hpp"
using std::move;
using std::nothrow;

#include <stdexcept>
using std::runtime_error;

namespace hgardenpi
{

    inline namespace v1
    {

        SystemConcrete::~SystemConcrete()
        {
            if (lockService)
            {
                delete lockService;
            }
        }

        void SystemConcrete::initialize()
        {
        }

        void SystemConcrete::start() {}

        ConfigInfo::Ptr SystemConcrete::getConfigInfo() const noexcept
        {
            ConfigSerivce config(HGARDENPI_FILE_CONFIG);

            configInfo = move(config.read());
        }

        const LockService *SystemConcrete::getLockService() const
        {
            if (!configInfo)
            {
                throw runtime_error("System non initialized");
            }
            if (!lockService)
            {
                lockService = new (nothrow) LockServiceConcrete;
                if (!lockService)
                {
                    throw runtime_error("no memory for lockService");
                }
            }
            return lockService;
        }

        const LogService &SystemConcrete::getLogService() const
        {
        }

        float SystemConcrete::getCPUTemperature() const
        {
            return ::hgardenpi::v1::getCPUTemperature();
        }

        DeviceInfo::Ptr SystemConcrete::getDeviceInfo() const
        {
            if (!deviceInfo)
            {
                deviceInfo = ::hgardenpi::v1::getDeviceInfo();
            }
            return deviceInfo;
        }

    }
}
