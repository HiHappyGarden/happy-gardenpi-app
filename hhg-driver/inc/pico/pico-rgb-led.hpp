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

#include "hhg-iface/rgb_led.hpp"

#include <pico/types.h>
#include <hardware/pio.h>

namespace hhg::driver
{
inline namespace v1
{

class pico_rgb_led final : public hhg::iface::rgb_led
{
public:
    enum pin : uint
    {
        RED = 13,
        GREEN = 14,
        BLUE = 15
    };

    ~pico_rgb_led() override = default;

    os::exit init(os::error **error) OS_NOEXCEPT override;

    void set_red(bool value) const OS_NOEXCEPT override;

    void set_green(bool value) const OS_NOEXCEPT override;

    void set_blue(bool value) const OS_NOEXCEPT override;

};

}
}