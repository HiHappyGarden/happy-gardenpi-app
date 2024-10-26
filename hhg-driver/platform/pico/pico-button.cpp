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


#include "pico/pico-button.hpp"
using namespace hhg::iface;
using namespace os;

#include <hardware/gpio.h>
#include <pico/stdio.h>

namespace hhg::driver
{
namespace v1
{
namespace
{
    constexpr char APP_TAG[] = "DRV BUTTON";
}

pico_button::pico_button() = default;
pico_button::~pico_button() = default;


os::exit pico_button::init(os::error **error)
{
    if(singleton)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    gpio_set_irq_enabled_with_callback(PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &pico_button::on_click);

    return thread.create();
}



void pico_button::on_click(uint gpio, uint32_t event_mask)
{
    static uint64_t last_click_press = 0;
    static uint64_t last_click_release = 0;
    uint64_t now = osal_system_current_time_millis();
    if (
            ( event_mask & GPIO_IRQ_EDGE_FALL && now - last_click_press > DEBOUNCE_TIME)
            || ( event_mask & GPIO_IRQ_EDGE_RISE && now - last_click_release > DEBOUNCE_TIME)
        )
    {

        struct click_event click_event { .timestamp = now };
        if(event_mask & GPIO_IRQ_EDGE_FALL)
        {
            click_event.status = status::PRESS;
            last_click_press = osal_system_current_time_millis();
        }
        else if(event_mask & GPIO_IRQ_EDGE_RISE)
        {
            click_event.status = status::RELEASE;
            last_click_release = osal_system_current_time_millis();
        }

        if(singleton->queue.post_from_isr(reinterpret_cast<const uint8_t *>(&click_event), 150_ms) == osal::exit::KO)
        {
            OSAL_LOG_DEBUG(APP_TAG, "Debounce detect");
        }
    }
}

void *pico_button::handle(void *arg)
{
    click_event event_last;
    while (singleton->run)
    {

        struct click_event click_event;
        if(singleton->queue.fetch(&click_event, WAIT_FOREVER) == osal::exit::OK)
        {
            if(singleton->obj && singleton->callback)
            {
                if(event_last.status == status::PRESS && click_event.status == status::RELEASE && click_event.timestamp - event_last.timestamp >= LONG_CLICK_TIME)
                {
                    (singleton->obj->*singleton->callback)(status::LONG_PRESS);
                    event_last.status = status::LONG_PRESS;
                    event_last = click_event;
                }
                else
                {
                    (singleton->obj->*singleton->callback)(click_event.status);
                    event_last = click_event;
                }


            }
        }
    }
    return nullptr;
}


}
}