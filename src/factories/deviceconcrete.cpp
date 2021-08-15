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
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

#include "../services/deviceservice.hpp"
#include "../threadengine.hpp"
#include "../components/lcd1602.hpp"
#include "../components/buttonconcrete.hpp"
#include "../components/relaymodule4channel.hpp"


namespace hgardenpi
{

    inline namespace v1
    {

        DeviceConcrete::~DeviceConcrete() noexcept
        {
            if (display)
            {
                delete display;
                display = nullptr;
            }
            if (button)
            {
                delete button;
                button = nullptr;
            }
            if (relayModule)
            {
                delete relayModule;
                relayModule = nullptr;
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
                throw runtime_error("hardware not supported, you need a Raspberry Pi Zero W");
            }

            //initialize WiringPI
            wiringPiSetup();

            display = new(nothrow) LCD1602(LCD1602::LCD_RS,
                                           LCD1602::LCD_E,
                                           LCD1602::LCD_D4,
                                           LCD1602::LCD_D5,
                                                LCD1602::LCD_D6,
                                                LCD1602::LCD_D7,
                                                LCD1602::LCD_CONTRAST);
            if (!display)
            {
                throw runtime_error("no memory for display");
            }

            button = new(nothrow) ButtonConcrete(ButtonConcrete::PIN);
            if (!button)
            {
                throw runtime_error("no memory for button");
            }

            relayModule = new(nothrow) RelayModule4Channel(RelayModule4Channel::IN1,
                                                           RelayModule4Channel::IN2,
                                                           RelayModule4Channel::IN3,
                                                           RelayModule4Channel::IN4);
            if (!relayModule)
            {
                throw runtime_error("no memory for button");
            }

            //display normalization
            display->setContrastTurnOn(false);
            display->clear();
        }

        void DeviceConcrete::start()
        {
            //set contrast management when click on button
            button->setInternalOnClick([&]
            {
                turnOnContrastDisplayFor();
            });

            //set display loop
            if (threadPool)
            {
                threadPool->enqueue([&]
                {
                    turnOnContrastDisplayFor(Time::DISPLAY_SHORT_TICK);

                    printOnDisplay("Happy|Garden PI", true);

                    threadSleep(Time::DISPLAY_SHORT_TICK);

                    turnOnContrastDisplayFor(Time::DISPLAY_CONTRAST_TICK);

                    while (wiringPiRunningThread)
                    {
                        printOnDisplayStandardInfo();
                    }
                });
            }


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

        void DeviceConcrete::printOnDisplayStandardInfo() const noexcept
        {

            printOnDisplay("MAC ADDRESS|" + getWlan0MAC(), true);

            threadSleep(Time::DISPLAY_TICK);

            auto &&ip = getWlan0IP();

            printOnDisplay(_("IP ADDRESS|") + (ip != "0:0:0:0" ? ip : _("not connected")), true);

            threadSleep(Time::DISPLAY_TICK);

            stringstream ss;
            ss << setprecision(2) << getCPUTemperature();

            printOnDisplay(_("INTERNAL TEMP|") + ss.str() + "C", true);

            threadSleep(Time::DISPLAY_TICK);

        }

        inline void DeviceConcrete::turnOnContrastDisplayFor(const Time &&wait) noexcept
        {
            if (contrastDisplayClicked)
            {
                return;
            }
            contrastDisplayClicked.store(true);
            if (threadPool)
            {
                threadPool->enqueue([&, wait]
                {
                    display->setContrastTurnOn(true);
                    threadSleep(wait);
                    display->setContrastTurnOn(false);
                    contrastDisplayClicked = false;
                });
            }

        }
    }
}