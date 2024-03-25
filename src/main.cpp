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

#include <FreeRTOS.h>
#include "osal/osal.hpp"
 #include "hhg-app/app-main.hpp"
#include "hhg-driver/hardware.hpp"
#include "hhg-driver/os-config.hpp"
// #include "hhg-utils/hhg-utils.hpp"
// #include "osal/osal.hpp"
#include <stdio.h>
#include "pico/stdlib.h"
#include "boards/pico_w.h"
#include "hardware/structs/systick.h"

#include "pico.h"
#include "task.h"
#include "timers.h"


// #include "stm32g4xx_hal.h"

// extern "C" uint8_t driver_lpuart_transmit(const uint8_t* data, uint16_t size);
// extern "C" I2C_HandleTypeDef hi2c1;
namespace
{

    constexpr const char APP_TAG[] = "MAIN";

    os::thread test_thread{
    "test thread"
    , hhg::driver::NORMAL
    , 1024
    , [](auto) -> void*
    {

        using namespace os;

        //bool check = true;

        while(true)
        {

            //auto i = hhg::utils::random_number(1, 1000);

            OS_LOG_DEBUG("--->", "1");

    //			uint8_t _data = check ? 0xFF : 0x00;
    //			HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, 0x27, &_data, 1, 100);

            os::us_sleep(1_s);
        }

        return nullptr;
    }};

 }

void led_task()
{
    while (true)
    {
        OS_LOG_INFO("TEST", "LED_PIN, 1");
        vTaskDelay(100);
        OS_LOG_INFO("TEST", "LED_PIN, 0");
        vTaskDelay(100);

    }
}

#ifdef STM32G474xx
int hhg_main(void)
#else
int main(int argc, char* argv[])
#endif
{
 	static os::error* error = nullptr;

 	static hhg::driver::hardware hw{&error};
 	static hhg::app::app_main app_main{hw, &error};
 	if(error)
 	{
 		os::printf_stack_error(APP_TAG, error);
 		delete error;
 	}

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

//     OS_LOG_INFO(APP_TAG, "Init app_main");
//     if(app_main.init(&error) == os::exit::KO)
//     {
//         if(error)
//         {
//             os::printf_stack_error(APP_TAG, error);
//             delete error;
//         }
//         os::stop_main_loop();
//         exit(EXIT_FAILURE);
//     }

//     OS_LOG_INFO(APP_TAG, "Start FSM app_main");
//     if(app_main.fsm_start(&error) == os::exit::KO)
//     {
//         if(error)
//         {
//             os::printf_stack_error(APP_TAG, error);
//             delete error;
//         }
//         os::stop_main_loop();
//         exit(EXIT_FAILURE);
//     }
//     OS_LOG_INFO(APP_TAG, "Start FSM app_main - OK");

//     test_thread.create(nullptr, &error);
//     if(error)
//     {

//         delete error;
//         return static_cast<int>(os::exit::KO);
//     }

 #ifndef STM32G474xx
     os::start_main_loop();
 #endif
    return static_cast<int>(os::exit::OK);

//    xTaskCreate(reinterpret_cast<TaskFunction_t>(led_task), "LED_Task", 256, NULL, 1, NULL);
//    vTaskStartScheduler();

    return 0;

}


