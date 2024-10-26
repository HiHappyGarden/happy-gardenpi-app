/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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
using rgb = hhg::iface::rgb_led::rgb;

#include <hardware/pwm.h>

namespace hhg::driver
{
inline namespace v1
{

    os::exit pico_rgb_led::init(os::error **error) OSAL_NOEXCEPT
    {
        pwm_set_gpio_level(RED, 0);
        pwm_set_gpio_level(GREEN, 0);
        pwm_set_gpio_level(BLUE, 0);
        return exit::OK;
    }

    void pico_rgb_led::set_red(uint8_t value) const OSAL_NOEXCEPT
    {
        rgb.red = value;
        pwm_set_gpio_level(RED, value * MULTI);
    }

    void pico_rgb_led::set_green(uint8_t value) const OSAL_NOEXCEPT
    {
        rgb.green = value;
        pwm_set_gpio_level(GREEN, value * MULTI);
    }

    void pico_rgb_led::set_blue(uint8_t value) const OSAL_NOEXCEPT
    {
        rgb.blue = value;
        pwm_set_gpio_level(BLUE, value * MULTI);
    }

    void pico_rgb_led::set_rgb(uint8_t red, uint8_t green, uint8_t blue) const OSAL_NOEXCEPT
    {
        rgb = {red, green, blue};
        pwm_set_gpio_level(RED, red * MULTI);
        pwm_set_gpio_level(GREEN, green * MULTI);
        pwm_set_gpio_level(BLUE, blue * MULTI);
    }

    const rgb& pico_rgb_led::get_rgb() const {
        return rgb;
    }

}
}