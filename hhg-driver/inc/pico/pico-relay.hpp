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

#include "hhg-iface/relay.hpp"

#include <pico/types.h>


namespace hhg::driver
{
inline namespace v1
{

class pico_relay final : public hhg::iface::relay
{
public:

    enum pin : uint
    {
        RELAY_0 = 6,
        RELAY_1 = 7,
        RELAY_2 = 8,
        RELAY_3 = 9,
    };

    pico_relay() OS_NOEXCEPT;
    ~pico_relay() OS_NOEXCEPT override;

    os::exit init(os::error **error) OS_NOEXCEPT override;

    uint8_t size() const OS_NOEXCEPT override;

    bool operator [](uint8_t idx) const OS_NOEXCEPT override;

    bool set(uint8_t pin, bool value) OS_NOEXCEPT override;

};

}
}