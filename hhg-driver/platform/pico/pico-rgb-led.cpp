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

#include "pico/pico-rgb-led.hpp"
using namespace os;

namespace hhg::driver
{
inline namespace v1
{

    os::exit pico_rgb_led::init(os::error **error) OS_NOEXCEPT
    {
        gpio_put(RED, true);
        gpio_put(GREEN, true);
        gpio_put(BLUE, false);
        return exit::OK;
    }

    void pico_rgb_led::set_red(bool value) const OS_NOEXCEPT
    {
        gpio_put(RED, value);
    }

    void pico_rgb_led::set_green(bool value) const OS_NOEXCEPT
    {
        gpio_put(GREEN, value);
    }

    void pico_rgb_led::set_blue(bool value) const OS_NOEXCEPT
    {
        gpio_put(BLUE, value);
    }

}
}