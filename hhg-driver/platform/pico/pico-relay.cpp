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

#include "pico/pico-relay.hpp"
using namespace os;

#include <hardware/gpio.h>

namespace hhg::driver
{
inline namespace v1
{

pico_relay::pico_relay() OSAL_NOEXCEPT = default;
pico_relay::~pico_relay() OSAL_NOEXCEPT = default;

os::exit pico_relay::init(error **error)
{

    gpio_put(RELAY_0, true);
    gpio_put(RELAY_1, true);
    gpio_put(RELAY_2, true);
    gpio_put(RELAY_3, true);

    return exit::OK;
}


inline uint8_t pico_relay::size() const OSAL_NOEXCEPT
{
    return 4;
}

bool pico_relay::operator[](uint8_t idx) const OSAL_NOEXCEPT
{
    if(idx >= size())
    {
        return false;
    }

    return !gpio_get(RELAY_0 + idx);;
}

bool pico_relay::set(uint8_t idx, bool value) OSAL_NOEXCEPT
{
    if(idx >= size())
    {
        return false;
    }

    gpio_put(RELAY_0 + idx, !value);

    return true;
}


}
}

