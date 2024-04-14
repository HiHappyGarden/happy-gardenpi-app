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

#include "hhg-driver/rotary-encored.hpp"
#include <pico/types.h>

namespace hhg::driver
{
inline namespace v1
{

class pico_rotary_encoder : public rotary_encoder
{

    static inline pico_rotary_encoder* singleton = nullptr;

public:

    enum pin : uint
    {
        ENCODER_A   = 21,
        ENCODER_B   = 20,
        ENCODER_BTN = 19
    };

    pico_rotary_encoder() OS_NOEXCEPT;
    ~pico_rotary_encoder() OS_NOEXCEPT override;

    os::exit init(os::error** error) OS_NOEXCEPT override;

    static void encoder_callback(uint gpio, uint32_t events);
};

}
}
