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

#include <stm32g4xx/stm32-lpuart.hpp>
using hhg::iface::io_on_receive;
using hhg::iface::io_source;

#include "stm32g4xx/driver-lpuart.h"
using namespace os;

namespace hhg::driver
{
inline namespace v1
{


stm32_lpuart::stm32_lpuart() OSAL_NOEXCEPT = default;

stm32_lpuart::~stm32_lpuart()
{
	singleton = nullptr;
}

os::exit stm32_lpuart::init(error** error) OSAL_NOEXCEPT
{
	if(singleton)
	{
		if(error)
		{
	        *error = OSAL_ERROR_BUILD("stm32_lpuart::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	singleton = this;
	driver_lpuart_register_rx_callback([](uint8_t ch)
	{

		if(stm32_lpuart::singleton && stm32_lpuart::singleton->obj && stm32_lpuart::singleton->on_receive_callback)
		{
			(stm32_lpuart::singleton->obj->*stm32_lpuart::singleton->on_receive_callback)(io_source::UART, &ch, 1);
		}

	});

	return exit::OK;
}


inline void stm32_lpuart::set_on_receive(const io_on_receive* obj, on_receive on_receive_callback) OSAL_NOEXCEPT
{
	this->obj = obj;
	this->on_receive_callback = on_receive_callback;
}

os::exit stm32_lpuart::transmit(const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT
{

	if(driver_lpuart_transmit(data, size) == static_cast<int>(exit::KO))
	{
		return exit::KO;
	}

	return exit::OK;
}

}
}
