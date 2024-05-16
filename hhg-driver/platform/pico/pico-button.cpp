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
            *error = OS_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    gpio_set_irq_enabled_with_callback(PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &pico_button::on_click);

    thread.create();

    return exit::OK;
}



void pico_button::on_click(uint gpio, uint32_t event_mask) {
    static uint64_t last_click_press = 0;
    static uint64_t last_click_release = 0;
    uint64_t now = osal_system_current_time_millis();
    if (
            ( event_mask & GPIO_IRQ_EDGE_FALL && now - last_click_press > DEBOUNCE_TIME)
            || ( event_mask & GPIO_IRQ_EDGE_RISE && now - last_click_release > DEBOUNCE_TIME)
        )
    {

        status status;
        if(event_mask & GPIO_IRQ_EDGE_FALL)
        {
            status = status::PRESS;
            last_click_press = osal_system_current_time_millis();
        }
        else if(event_mask & GPIO_IRQ_EDGE_RISE)
        {
            status = status::RELEASE;
            last_click_release = osal_system_current_time_millis();
        }

        if(singleton->queue.post_from_isr(reinterpret_cast<const uint8_t *>(&status), 100_ms) == osal::exit::KO)
        {
            OS_LOG_DEBUG(APP_TAG, "Debounce detect");
        }
    }



}

    void *pico_button::encoder_handle(void *arg)
    {
        while (singleton->run)
        {

            status status;
            if(singleton->queue.fetch(&status, WAIT_FOREVER) == osal::exit::OK)
            {
                if(singleton->obj && singleton->callback)
                {
                    (singleton->obj->*singleton->callback)(status);
                }
            }
        }
        return nullptr;
    }


}
}