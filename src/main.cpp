/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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

#ifdef HHG_NO_MAIN
#include "main.h"
#endif
#include "osal/osal.hpp"
#include "hhg-driver/os-config.hpp"
#include "hhg-app/app-main.hpp"
#include "hhg-driver/hardware.hpp"

namespace
{
    constexpr const char APP_TAG[] = "MAIN";

    void* main_thread_handler(void* arg);

    os::thread main_thread{"main", hhg::driver::REALTIME, 1'024, main_thread_handler};

    void* main_thread_handler(void* arg)
    {

        static os::error* error = nullptr;

        static hhg::driver::hardware hw{&error};
        static hhg::app::app_main app_main{hw, &error};
        if(error)
        {
            os::printf_stack_error(APP_TAG, error);
            delete error;
        }

        if(hw.init(&error) == os::exit::KO)
        {
            if(error)
            {
                os::printf_stack_error(APP_TAG, error);
                delete error;
            }
            return nullptr;
        }

        if(app_main.init(&error) == os::exit::KO)
        {
            if(error)
            {
                os::printf_stack_error(APP_TAG, error);
                delete error;
            }
            os::stop_main_loop();
            exit(EXIT_FAILURE);
        }

        OS_LOG_INFO(APP_TAG, "Start FSM app_main");
        if(app_main.fsm_start(&error) == os::exit::KO)
        {
            if(error)
            {
                os::printf_stack_error(APP_TAG, error);
                delete error;
            }
            os::stop_main_loop();
            exit(EXIT_FAILURE);
        }

        main_thread.exit();

        return nullptr;
    }
}


#ifdef HHG_NO_MAIN
int hhg_main(void)
#else
int main(int argc, char* argv[])
#endif
{

    main_thread.create();

    os::start_main_loop();

    return static_cast<int>(os::exit::OK);
}


