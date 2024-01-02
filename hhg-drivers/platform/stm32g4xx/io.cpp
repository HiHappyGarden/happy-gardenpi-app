/*
 * io.cpp
 *
 *  Created on: Jan 2, 2024
 *      Author: antoniosalsi
 */

#include "stm32g4xx/io.hpp"
#include "stm32g4xx/driver-lpuart.h"

namespace hhg::driver
{
inline namespace v1
{


io::io() = default;

io::~io() = default;


os::exit io::init(os::error** error) OS_NOEXCEPT
{


	return os::exit::OK;
}


inline void io::set_on_read(on_read on_read_callback) OS_NOEXCEPT
{
	this->on_read_callback = on_read_callback;
}

os::exit io::write(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{

	if(driver_lpuart_transmit(data, size) == static_cast<int>(os::exit::KO))
	{
		return os::exit::KO;
	}

	return os::exit::OK;
}

}
}
