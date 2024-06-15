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

namespace hhg::driver
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "HARDWARE";

}

hardware::hardware(class error** error) OSAL_NOEXCEPT
{

}

os::exit hardware::init(error** error) OSAL_NOEXCEPT
{
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

