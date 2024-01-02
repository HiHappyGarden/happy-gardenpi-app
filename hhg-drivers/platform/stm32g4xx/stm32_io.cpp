/*
 * io.cpp
 *
 *  Created on: Jan 2, 2024
 *      Author: antoniosalsi
 */

#include "stm32g4xx/stm32_io.hpp"
#include "stm32g4xx/driver-lpuart.h"
using namespace os;

namespace hhg::driver
{
inline namespace v1
{


stm32_io::stm32_io() = default;

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
	driver_lpuart_register_rx_callback([](auto ch)
	{
		if(singleton->on_read_callback)
		{
			singleton->source = LPUART;
			singleton->on_read_callback(&ch, 1);
		}
	});

	return exit::OK;
}


inline void stm32_io::set_on_read(on_read on_read_callback) OS_NOEXCEPT
{
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
