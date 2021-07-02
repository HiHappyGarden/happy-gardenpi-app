/*
MIT License

Copyright (c) 2021 Happy GardenPI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "globals.hpp"

#include <wiringPi.h>
#include <mosquittopp.h>

#include <thread>
using namespace std;

#include "services/lockservice.hpp"
using std::make_unique;

#include "services/logservice.hpp"
#include "config.h"
#include "constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        Globals::Globals() noexcept
        try
        {
            lockService = make_unique<LockService>();
        }
        catch (const std::bad_alloc &e)
        {
            LogService::getInstance()->write(LOG_ERR, e.what());
        }

        void initialize()
        {

            //check if already run an instance of Happy GardenPI
            if (Globals::getInstance()->lockService->lock())
            {
                char error[] = "hardware not supporrted, you need a Raspberry Pi Zero W";
                LogService::getInstance()->write(LOG_ERR, "cpu: %d", error);
                throw runtime_error(error);
            }

            //print init info on log
            LogService::getInstance()->write(LOG_INFO, "version: %s", HGARDENPI_VER);

            //get device info
            Globals::getInstance()->deviceInfo = getDeviceInfo();

            //print device info to log
            LogService::getInstance()->write(LOG_INFO, "hardware: %s", Globals::getInstance()->deviceInfo->hardhare.c_str());
            LogService::getInstance()->write(LOG_INFO, "revision: %s", Globals::getInstance()->deviceInfo->revision.c_str());
            LogService::getInstance()->write(LOG_INFO, "serial: %s", Globals::getInstance()->deviceInfo->serial.c_str());
            LogService::getInstance()->write(LOG_INFO, "model: %s", Globals::getInstance()->deviceInfo->model.c_str());
            LogService::getInstance()->write(LOG_INFO, "cpu: %d", Globals::getInstance()->deviceInfo->cpu);

            //HW check
            if (Globals::getInstance()->deviceInfo->hardhare != HW_V1)
            {
                char error[] = "hardware not supporrted, you need a Raspberry Pi Zero W";
                LogService::getInstance()->write(LOG_ERR, "cpu: %d", error);
                throw runtime_error(error);
            }

            //initialize WiringPI
            wiringPiSetupGpio();

            //initialize mosquittopp
            mosqpp::lib_init();
        }

        [[noreturn]] void start()
        {

            while (true)
            {
                this_thread::sleep_for(chrono::milliseconds(static_cast<int64_t>(Time::TICK)));
            }
        }
    }
}