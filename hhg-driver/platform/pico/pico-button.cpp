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
#include <pico/stdio.h>
#include <pico/stdlib.h>

namespace hhg::driver
{
namespace v1
{
pico_button::~pico_button() = default;
pico_button::pico_button()= default;

os::exit pico_button::init(os::error **error)
{
    if(singleton)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    gpio_set_irq_enabled_with_callback(PIN, GPIO_IRQ_EDGE_FALL, true, &pico_button::on_click);
    return exit::OK;
}



void pico_button::on_click(uint gpio, uint32_t event_mask)
{
    OS_LOG_INFO("...", "--->%d %d", gpio, event_mask);

    if(!fall && gpio == pin::PIN && event_mask == GPIO_IRQ_EDGE_FALL)
    {
        alarm_id = add_alarm_in_us(100, [](alarm_id_t id, void *user_data) -> int64_t
        {
            fall = true;
            alarm_id = 0;
            return 0;
        },
        nullptr, true);

    }


        if(singleton->obj && singleton->callback)
        {
            (singleton->obj->*singleton->callback)();
        }
        fall = false;


}


}
}