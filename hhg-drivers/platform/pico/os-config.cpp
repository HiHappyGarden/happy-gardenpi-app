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

#include "hhg-driver/os-config.hpp"
using namespace os;
#include "pico/pico-gpio.hpp"
#include "cJSON.h"

#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <hardware/structs/systick.h>
#include "hardware/clocks.h"

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



os::exit os_config_init() OS_NOEXCEPT
{
    stdio_init_all();

    cJSON_InitHooks(&cjson_hooks);

    init_gpio();

	return exit::OK;
}

extern "C" uint64_t osal_system_current_time_millis()
{
    return time_us_32();
}

extern "C" void vApplicationStackOverflowHook(TaskHandle_t task, char *)
{
    (void)task;

    system("reboot");
    for(;;);
}

extern "C" void vApplicationMallocFailedHook( void )
{
    system("reboot");
    for(;;);
}



}
}


