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

#include "stm32g4xx/stm32_io.hpp"
using hhg::iface::io_on_read;

#include "stm32g4xx/driver-lpuart.h"
using namespace os;

namespace hhg::driver
{
inline namespace v1
{


stm32_io::stm32_io() OS_NOEXCEPT = default;

stm32_io::~stm32_io()
{
	singleton = nullptr;
}

os::exit stm32_io::init(error** error) OS_NOEXCEPT
{
	if(singleton)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("stm32_io::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	singleton = this;
	driver_lpuart_register_rx_callback([](uint8_t ch)
	{

		if(stm32_io::singleton && singleton->obj && stm32_io::singleton->on_read_callback)
		{
			stm32_io::singleton->source = stm32_io::LPUART;
			(singleton->obj->*singleton->on_read_callback)(&ch, 1);
		}

	});

	return exit::OK;
}


inline void stm32_io::set_on_read(const io_on_read* obj, on_read on_read_callback) OS_NOEXCEPT
{
	this->obj = obj;
	this->on_read_callback = on_read_callback;
}

os::exit stm32_io::write(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{
	switch(source)
	{
	case LPUART:
		if(driver_lpuart_transmit(data, size) == static_cast<int>(exit::KO))
		{
			return exit::KO;
		}
		break;
	}

	return exit::OK;
}

}
}
