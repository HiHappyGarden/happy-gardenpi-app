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

#include "pico/pico-i2c.hpp"
using hhg::iface::io_on_receive;
using hhg::iface::io_source;
using namespace os;

#include <pico/binary_info.h>
#include <hardware/gpio.h>


namespace hhg::driver
{
inline namespace v1
{

pico_i2c::pico_i2c() OS_NOEXCEPT = default;

pico_i2c::~pico_i2c()
{
	singleton = nullptr;
}

os::exit pico_i2c::init(error** error) OS_NOEXCEPT
{
	if(singleton)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("pico_i2c::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	singleton = this;

    i2c_init(i2c_default, 100 * 1'000);

    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

	return exit::OK;
}


inline void pico_i2c::set_on_receive(const io_on_receive* obj, on_receive on_receive_callback) OS_NOEXCEPT
{
	this->obj = obj;
	this->on_receive_callback = on_receive_callback;
}

os::exit pico_i2c::transmit(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{

	return exit::OK;
}

}
}
