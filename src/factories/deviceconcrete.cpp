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
#include "../utilities/threadpool.hpp"
#include "../components/lcd1602.hpp"
#include "../components/buttonconcrete.hpp"


#include <iomanip>
using std::setprecision;


using namespace std;


namespace hgardenpi
{

    inline namespace v1
    {

        extern void threadSleep(volatile bool &run, Time &&millis) noexcept;

        extern void threadSleep(volatile bool &run, const Time &millis) noexcept;

        DeviceConcrete::~DeviceConcrete() noexcept
        {
            if (display)
            {
                delete display;
                display = nullptr;
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

            display = new(std::nothrow) LCD1602(LCD1602::LCD_RS, LCD1602::LCD_E, LCD1602::LCD_D4, LCD1602::LCD_D5,
                                                LCD1602::LCD_D6, LCD1602::LCD_D7, LCD1602::LCD_CONTRAST);
            if (!display)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("no memory for display")
            }

            button = new(std::nothrow) ButtonConcrete(ButtonConcrete::PIN);
            if (!button)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("no memory for button")
            }

        }

        /**
         * @brief Start main look and scheduler
         * @param run check if the loops are in execution
         * @exception runtime_error when hardware requisites mismatch
         */
        void DeviceConcrete::start(volatile bool &run)
        {
            //set contrast management when click on button
            button->setInternalOnClick([&]
                                       {
                                           turnOnContrastDisplayFor(run);
                                       });

            //set display loop
            threadPool->enqueue([&]
                                {
                                    //show welcome message

                                    turnOnContrastDisplayFor(run, Time::DISPLAY_SHORT_TICK);

                                    printOnDisplay("Happy|Garden PI", true);

                                    threadSleep(run, Time::DISPLAY_SHORT_TICK);

                                    turnOnContrastDisplayFor(run);

                                    while (run)
                                        printOnDisplayStandardInfo(run);
                                });

        }

        inline float DeviceConcrete::getCPUTemperature() const
        {
            return ::hgardenpi::v1::getCPUTemperature();
        }

        inline DeviceInfo::Ptr DeviceConcrete::getInfo() const
        {
            return deviceInfo;
        }

        void DeviceConcrete::printOnDisplay(const string &txt, bool enableFormat) const noexcept
        {
            display->print("");
            if (!enableFormat)
            {
                display->print(txt);
                return;
            }
            stringstream ss(txt);
            vector<string> result;

            while (ss.good())
            {
                string substr;
                getline(ss, substr, DIVISOR);
                result.push_back(substr);
            }


            uint8_t i = 0;
            for (auto &it : result)
            {
                if (i >= display->getRows())
                {
                    break;
                }
                decltype(auto) diff = display->getColls() - it.size();
                if (diff > 0)
                {
                    auto &&row = result[i];
                    for (int count = 0; count < static_cast<int>(diff / 2) + (diff % 2 ? 1 : 0); count++)
                    {
                        row = move(" " + row);
                    }
                    for (int count = 0; count < static_cast<int>(diff / 2); count++)
                    {
                        row += " ";
                    }
                    display->print(row);
                } else
                {
                    display->print(txt);
                    break;
                }
                i++;
            }

        }

        void DeviceConcrete::printOnDisplayStandardInfo(volatile bool &run) const noexcept
        {
            printOnDisplay("MAC ADDRESS|" + getWlan0MAC(), true);

            threadSleep(run, Time::DISPLAY_TICK);

            auto &&ip = getWlan0IP();

            printOnDisplay(_("IP ADDRESS|") + (ip != "0:0:0:0" ? ip : _("not connected")), true);

            threadSleep(run, Time::DISPLAY_TICK);

            stringstream ss;
            ss << setprecision(2) << getCPUTemperature();

            printOnDisplay(_("INTERNAL TEMP|") + ss.str() + "C", true);

            threadSleep(run, Time::DISPLAY_TICK);

        }

        inline void DeviceConcrete::turnOnContrastDisplayFor(volatile bool &run, const Time &&wait) noexcept
        {
            threadPool->enqueue([&]
                                {
                                    display->setContrastTurnOn(true);
                                    threadSleep(run, wait);
                                    display->setContrastTurnOn(false);
                                });

        }
    }
}