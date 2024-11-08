/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-driver/os-config.hpp"
using namespace os;
#include "pico/pico-gpio.hpp"
#include "cJSON.h"

#include <FreeRTOS.h>
#include <timers.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

uint64_t FreeRTOSRunTimeTicks;

static timer run_time_tick_timer{ 100_ms,[] (timer*, void*)-> void*
{
    FreeRTOSRunTimeTicks++;
    return nullptr;
}};

extern "C" void ConfigureTimerForRunTimeStats(void)
{
    FreeRTOSRunTimeTicks = 0;
    run_time_tick_timer.create();
    run_time_tick_timer.start();
}

extern "C" uint64_t osal_system_current_time_millis()
{
    return xTaskGetTickCount();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task, char *)
{
    (void)task;

    //system("reboot");
    printf("vApplicationStackOverflowHook()");
#if HHG_WIFI_DISABLE == 0
    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
#else
    for(;;);
#endif
}

extern "C" void vApplicationMallocFailedHook( void )
{
    //system("reboot");
    printf("vApplicationMallocFailedHook()");
#if HHG_WIFI_DISABLE == 0
    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(500);
    }
#else
    for(;;);
#endif
}

namespace hhg::driver
{
inline namespace v1
{

uint32_t const LOWEST = 1;
uint32_t const LOW = 2;
uint32_t const NORMAL = 3;
uint32_t const HIGH = 4;
uint32_t const HIGHEST = 5;
uint32_t const REALTIME = 6;

uint32_t const MINIMAL_STACK_SIZE = configMINIMAL_STACK_SIZE;

char const TIME_T_STR[] = "%lu";

namespace
{

cJSON_Hooks cjson_hooks
{
    .malloc_fn = [](auto size)
    {
        return pvPortMalloc(size);
    },
    .free_fn = [](auto ptr)
    {
        vPortFree(ptr);
    }
};

}

os::exit os_config_init() OSAL_NOEXCEPT
{
    stdio_init_all();

    cJSON_InitHooks(&cjson_hooks);

    init_gpio();

	return exit::OK;
}

}
}


