/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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
using namespace os;

#include <pico/binary_info.h>
#include <hardware/gpio.h>


namespace hhg::driver
{
inline namespace v1
{

pico_i2c::pico_i2c(i2c_inst_t* i2C_reference, uint speed) OSAL_NOEXCEPT
: i2C_reference(i2C_reference)
, speed(speed)
{}

pico_i2c::~pico_i2c() = default;

os::exit pico_i2c::init(error** error) OSAL_NOEXCEPT
{
    i2c_init(i2C_reference, speed);
    bi_decl(bi_2pins_with_func(pico_i2c::pin::SDA_PIN, pico_i2c::pin::SCL_PIN, GPIO_FUNC_I2C));

	return exit::OK;
}


}
}
