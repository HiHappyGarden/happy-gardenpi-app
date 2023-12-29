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

#include "hhg/main.h"
#include "osal/osal.hpp"
using namespace os;

#include <stdint.h>
#include "stm32g4xx_hal.h"

extern UART_HandleTypeDef hlpuart1;

namespace
{
os::thread test{"test", 4, 2048, [](auto args) -> void*
{

	string test = "ciao\n";

	while(1)
	{

		for(uint8_t i = 0; i < test.length(); i++)
		{
			if(HAL_UART_Transmit(&hlpuart1, (uint8_t*)&test[i], 1, 1000)!= HAL_OK)
			{
			  /* Transfer error in transmission process */

			}
		}



		os::us_sleep(500_ms);
	}

	return nullptr;
}};

}

#ifdef STM32G474xx
int hhg_main(void)
#else
int main(int argc, char* argv[])
#endif
{

	test.create();

	return static_cast<int>(os::exit::OK);
}
