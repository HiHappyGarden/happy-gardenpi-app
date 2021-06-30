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

#include "services/lockservice.hpp"
#include "services/logservice.hpp"
#include "config.h"

namespace hgardenpi
{
    inline namespace v1
    {
        using std::make_unique;

        Globals::Globals() noexcept
        try
        {
            lockService = make_unique<LockService>();
        }
        catch (const std::bad_alloc &e)
        {
            lockServicePassThrough = true;
            LogService::getInstance()->write(LOG_ERR, e.what());
        }

        void initialize()
        {

            LogService::getInstance()->write(LOG_INFO, "version: %s", HGARDENPI_VER);

            //check if already run an instance of Happy GardenPI

            //initialize WiringPI
            wiringPiSetupGpio();

            //initialize mosquittopp
            mosqpp::lib_init();
        }
    }
}