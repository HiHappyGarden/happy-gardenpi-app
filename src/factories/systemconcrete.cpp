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
using std::to_string;

#include <stdexcept>
using std::runtime_error;

#include "../services/schedulerconcrete.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        SystemConcrete::~SystemConcrete()
        {

            if (lockService)
            {
                release();
                delete lockService;
                lockService = nullptr;
            }
            if (logService)
            {
                delete logService;
                logService = nullptr;
            }
            if (scheduler)
            {
                delete scheduler;
                scheduler = nullptr;
            }
        }

        void SystemConcrete::initialize()
        {
            //load configuration
            ConfigService config(HGARDENPI_FILE_CONFIG);
            configInfo = move(config.read());

            if (!configInfo)
            {
                throw runtime_error(_("system non initialized"));
            }

            //initialize log service
            logService = new (nothrow) LogServiceConcrete;
            if (!logService)
            {
                throw runtime_error(_("no memory for lockService"));
            }

            //initialize lock service
            lockService = new (nothrow) LockServiceConcrete(configInfo);
            if (!lockService)
            {
                throw runtime_error(_("no memory for lockService"));
            }

            //check if already run an instance of Happy GardenPI
            if (lockService->lock())
            {
                string error(_("another instance already run pid:"));
                error += to_string(lockService->getPidInExecution());
                logService->write(LOG_ERR, "%s", error.c_str());
                throw runtime_error(error);
            }

            //write sw vertionb in log
            logService->write(LOG_INFO, "version: %s", HGARDENPI_VER);

            scheduler = new SchedulerConcrete(threadPool);
        }

        void SystemConcrete::start(volatile bool &run) {}

        inline void SystemConcrete::release() noexcept
        {
            lockService->release();
        }

    }
}
