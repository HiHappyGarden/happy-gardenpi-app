/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/


#include "hhg-platform/hardware.hpp"
#include "hhg-app/appmain.hpp"
#include "osal/osal.hpp"



#include <stdio.h>

namespace
{

constexpr const char APP_TAG[] = "MAIN";

}

int main(int argc, char* argv[]) try
{
    os::error* error = nullptr;

    hhg::intf::hardware&& hardware = hhg::platform::hardware();
    hhg::app::app_main app_main;

    OS_LOG_DEBUG(APP_TAG, "Start hardware");
    if(!hardware.init(&error))
    {
        if(error)
        {
            os::printf_stack_error(error);
            delete error;
        }
        os::stop_main_loop();
        exit(1);
    }

    OS_LOG_DEBUG(APP_TAG, "Start app_main");
    if(!app_main.init(&error))
    {
        if(error)
        {
            os::printf_stack_error(error);
            delete error;
        }
        os::stop_main_loop();
        exit(1);
    }

    OS_LOG_DEBUG(APP_TAG, "Start fsm");
    if(!app_main.fsm_start(&error))
    {
        if(error)
        {
            os::printf_stack_error(error);
            delete error;
        }
        os::stop_main_loop();
        exit(1);
    }

    os::start_main_loop();

    return 0;
}
catch(...)
{
    fprintf(stderr, "Unknown failure occurred. Possible memory corruption");
}
