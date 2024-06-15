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
#include "hhg-driver/os-config.hpp"
#include "stm32g4xx/driver-lpuart.h"
#include "stm32g4xx/stm32-fs-io.hpp"
#include "stm32g4xx/stm32-lpuart.hpp"
#include "stm32g4xx/stm32-time.hpp"
using namespace os;
using namespace hhg::iface;


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

hardware::hardware(class error** error) OSAL_NOEXCEPT
: uart(new hhg::driver::stm32_lpuart)
, fsio(new hhg::driver::stm32_fsio(static_cast<uint32_t>(addr_flash::PAGE_112), static_cast<uint32_t>(addr_flash::PAGE_127) + FLASH_PAGE_SIZE - 1 ))
, time(new hhg::driver::stm32_time)
{
	if(uart.get() == nullptr && error)
	{
        *error = OSAL_ERROR_BUILD("io(new hhg::driver::stm32_io) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
	}

	if(fsio.get() == nullptr && error)
	{
        *error = OSAL_ERROR_BUILD("io(new hhg::driver::stm32_fsio) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
	}

	if(time.get() == nullptr && error)
	{
        *error = OSAL_ERROR_BUILD("io(new hhg::driver::stm32_timer) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
	}
}

os::exit hardware::init(error** error) OSAL_NOEXCEPT
{
	OSAL_LOG_INFO(APP_TAG, "Init OS Config");
	if(os_config_init() == os::exit::KO)
	{
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init OS Config - OK");

	OSAL_LOG_INFO(APP_TAG, "Init PLUART");
	if(driver_lpuart_init() == EXIT_FAILURE)
	{
		if(error)
		{
	        *error = OSAL_ERROR_BUILD("driver_lpuart_init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init PLUART - OK");

	OSAL_LOG_INFO(APP_TAG, "Init IO");
	if(uart->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OSAL_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init IO - OK");

	OSAL_LOG_INFO(APP_TAG, "Init FS IO");
	if(fsio->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OSAL_ERROR_APPEND(*error, "fsio::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init FS IO - OK");


	return exit::OK;
}

const string<128>& hardware::get_info() OSAL_NOEXCEPT
{
	static string<128> ret;

	return ret;
}


const string<128>& hardware::get_version() OSAL_NOEXCEPT
{
	static string<128> ret;

	return ret;
}


}
}

