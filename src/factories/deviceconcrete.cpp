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

#include <sstream>
using std::stringstream;

#include "../services/deviceservice.hpp"
#include "../components/lcd1602.hpp"
#include "../utilities/threadpool.hpp"


#include <iostream>
using namespace std;


namespace hgardenpi
{

    inline namespace v1
    {

        DeviceConcrete::~DeviceConcrete() noexcept
        {
            if (display)
            {
                delete display;
            }
        }

        /**
         * @brief Before all call this function fo initialize the project
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
            wiringPiSetup();

            display = new (std::nothrow) LCD1602(LCD1602::LCD_RS, LCD1602::LCD_E, LCD1602::LCD_D4, LCD1602::LCD_D5, LCD1602::LCD_D6, LCD1602::LCD_D7, LCD1602::LCD_CONTRAST);
            if (!display)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("no memory for display")
            }

        }

        /**
         * @brief Start main look and scheduler
         * @param run check if the loops are in execution
         * @exception runtime_error when hardware requisites mismatch
         */
        void DeviceConcrete::start(volatile bool &run)
        {
            threadPool->enqueue([&]() {
                display->setContrastTurnOn(true);
                while (run)
                {

                }
                display->setContrastTurnOn(false);
                std::cout << "pippo" << endl;
            });



        }

        void DeviceConcrete::release() noexcept
        {
        }

        inline float DeviceConcrete::getCPUTemperature() const
        {
            return ::hgardenpi::v1::getCPUTemperature();
        }

        inline DeviceInfo::Ptr DeviceConcrete::getInfo() const
        {
            return deviceInfo;
        }

        void DeviceConcrete::printOnDisplay(const string &txt, bool splitByDivisor) const noexcept
        {
            if (!splitByDivisor)
            {
                display->print(txt);
                return;
            }
            stringstream ss( txt);
            vector<string> result;

            while( ss.good() )
            {
                string substr;
                getline( ss, substr, DIVISOR);
                result.push_back( substr );
            }

            if (result.size() == 2)
            {
                uint8_t i = 0;
                for (auto &it : result)
                {
                    if (i >= display->getRows())
                    {
                        break;
                    }
                    decltype(auto) diff = display->getColls() - it.size();
                    if (diff > 0) {
                        auto &&row = result[i];
                        for (int count = 0; count < static_cast<int>(diff / 2) + (diff % 2 ? 1 : 0) ; count++) {
                            row = move(" " + row);
                        }
                        for (int count = 0; count < static_cast<int>(diff / 2); count++) {
                            row += " ";
                        }
                        display->print(row);
                    } else {
                        display->print(txt);
                        break;
                    }
                    i++;
                }

            }
            else
            {
                display->print(txt);
            }
        }

        void DeviceConcrete::printOnDisplayStandardInfo() noexcept
        {
            std::cout << "start" << endl;
            printOnDisplay("MAC ADDRESS|" + getWlan0MAC(), true);

            this_thread::sleep_for(chrono::seconds(static_cast<int64_t>(Time::DISPLAY_TICK)));

            display->print("");

            this_thread::sleep_for(chrono::milliseconds (static_cast<int64_t>(Time::DISPLAY_PAUSE)));

            printOnDisplay("IP ADDRESS|" + getWlan0IP(), true);

            this_thread::sleep_for(chrono::seconds(static_cast<int64_t>(Time::DISPLAY_TICK)));

            display->print("");

            this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::DISPLAY_PAUSE)));
            std::cout << "end" << endl;
        }
    }
}