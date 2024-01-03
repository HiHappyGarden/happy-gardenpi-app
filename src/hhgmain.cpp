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

#include "hhg/main.h"
#include "hhg-driver/hardware.hpp"
#include "hhg-app/appmain.hpp"
#include "osal/osal.hpp"

extern "C" uint8_t driver_lpuart_transmit(const uint8_t* data, uint16_t size);

namespace
{

constexpr const char APP_TAG[] = "MAIN";

os::thread test_thread{
	"test thread"
	, 3
	, 1024
	, [](auto) -> void*
	{

		using namespace os;

		while(true)
		{
			OS_LOG_DEBUG("--->", "printf");
			os::us_sleep(1_s);
		}

		return nullptr;
	}};

hhg::driver::hardware hw{nullptr};
hhg::app::app_main app_main{hw};

}

#ifdef STM32G474xx
int hhg_main(void)
#else
int main(int argc, char* argv[])
#endif
{

    os::error* error = nullptr;

    OS_LOG_INFO(APP_TAG, "Init hardware");
    if(hw.init(&error) == os::exit::KO)
    {
        if(error)
        {
            os::printf_stack_error(APP_TAG, error);
            delete error;
        }
        return static_cast<int>(os::exit::KO);
    }

    OS_LOG_INFO(APP_TAG, "Init app_main");
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

//
//    test_thread.create(nullptr, &error);
//    if(error)
//    {
//        os::printf_stack_error(APP_TAG, error);
//        delete error;
//        return static_cast<int>(os::exit::KO);
//    }



//    OS_LOG_INFO(APP_TAG, "Start fsm");
//    if(pp_main.fsm_start(&error)  == os::exit::KO)
//    {
//        if(error)
//        {
//            os::printf_stack_error(APP_TAG, error);
//            delete error;
//        }
//        os::stop_main_loop();
//        exit(EXIT_FAILURE);
//    }
#ifndef STM32G474xx
    os::start_main_loop();
#endif


	return static_cast<int>(os::exit::OK);
}


