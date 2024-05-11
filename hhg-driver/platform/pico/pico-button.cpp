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


#include "pico/pico-button.hpp"
using namespace hhg::iface;
using namespace os;

#include <hardware/gpio.h>

namespace hhg::driver
{
namespace v1
{
pico_button::~pico_button() = default;
pico_button::pico_button()= default;

os::exit pico_button::init(os::error **error)
{
    gpio_set_irq_enabled_with_callback(PIN, GPIO_IRQ_EDGE_FALL, true, nullptr);
    return exit::OK;
}



}
}