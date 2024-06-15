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
    constexpr static uint16_t MULTI = UINT16_MAX / UINT8_MAX;

    mutable hhg::iface::rgb_led::rgb rgb;
public:
    enum pin : uint
    {
        RED = 13,
        GREEN = 14,
        BLUE = 15
    };

    ~pico_rgb_led() override = default;

    os::exit init(os::error **error) OSAL_NOEXCEPT override;

    void set_red(uint8_t value) const OSAL_NOEXCEPT override;

    void set_green(uint8_t value) const OSAL_NOEXCEPT override;

    void set_blue(uint8_t value) const OSAL_NOEXCEPT override;

    void set_rgb(uint8_t red, uint8_t green, uint8_t blue) const OSAL_NOEXCEPT override;

    const rgb_led::rgb& get_rgb() const OSAL_NOEXCEPT override;

};

}
}