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

#include "hhg-driver/hardware.hpp"
#include "stm32g4xx/driver-lpuart.h"
#include "stm32g4xx/io.hpp"
using namespace os;

#include "stm32g4xx_hal.h"

extern "C" uint64_t osal_system_current_time_millis()
{
	return HAL_GetTick();
}


namespace hhg::driver
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "HARDWARE";

}


hardware::hardware() OS_NOEXCEPT
: io(new hhg::driver::io)
{

}

os::exit hardware::init(os::error** error) OS_NOEXCEPT
{
	OS_LOG_INFO(APP_TAG, "Init PLUART");
	if(driver_lpuart_init() == EXIT_FAILURE)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("driver_lpuart_init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init PLUART - OK");

	OS_LOG_INFO(APP_TAG, "Init IO");
	if(io->init(error) == exit::KO)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("io::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init IO - OK");

	return exit::OK;
}

const string<128>& hardware::get_info() OS_NOEXCEPT
{
	static string<128> ret;

	return ret;
}


const string<128>& hardware::get_version() OS_NOEXCEPT
{
	static string<128> ret;

	return ret;
}


}
}

