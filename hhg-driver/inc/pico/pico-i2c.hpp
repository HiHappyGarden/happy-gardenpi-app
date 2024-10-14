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


#pragma once

#include "hhg-iface/i2c.hpp"

#include <hardware/i2c.h>


namespace hhg::driver
{
inline namespace v1
{

class pico_i2c final : public hhg::iface::i2c
{

    i2c_inst_t* i2C_reference = i2c1;
    uint speed = pico_i2c::SPEED;

    public:

    enum pin : uint
    {
        SDA_PIN = 2,
        SCL_PIN = 3
    };

    static constexpr uint SPEED = 100'000;

    explicit pico_i2c(i2c_inst_t* i2C_reference = i2c1, uint speed = pico_i2c::SPEED) OSAL_NOEXCEPT;
	~pico_i2c() OSAL_NOEXCEPT override;
	OSAL_NO_COPY_NO_MOVE(pico_i2c)

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

    i2c_inst_t const * get_i2C_reference() const OSAL_NOEXCEPT override
    {
        return i2C_reference;
    }

};

}
}
