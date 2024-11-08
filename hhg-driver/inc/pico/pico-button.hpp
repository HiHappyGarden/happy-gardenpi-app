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



#pragma once

#include "hhg-iface/button.hpp"
#include "hhg-driver/os-config.hpp"
#include "FreeRTOSConfig.h"

#include <pico/types.h>
#include <pico/time.h>

namespace hhg::driver
{
inline namespace v1
{


class pico_button : public hhg::iface::button
{

    static inline pico_button* singleton = nullptr;
    static constexpr uint16_t DEBOUNCE_TIME = 200;
    static constexpr uint16_t LONG_CLICK_TIME = 500;

    bool run = true;
    os::thread thread{"pico_button", hhg::driver::NORMAL, 1'024, handle}; //keep this amount of ram

    event *obj = nullptr;
    hhg::iface::button::event::callback callback = nullptr;

    struct click_event final
    {
        enum status status;
        uint64_t timestamp = 0;
    };

    os::queue queue{1, sizeof(click_event) };
public:

    enum pin : uint
    {
        PIN = 22

    };

    pico_button();
    ~pico_button() override;
    OSAL_NO_COPY_NO_MOVE(pico_button);

    inline void set_on_button_click(event *obj, event::callback callback) OSAL_NOEXCEPT override
    {
        this->obj = obj;
        this->callback = callback;
    }

    os::exit init(os::error **error) OSAL_NOEXCEPT override;

private:
    static void* handle(void* arg);
    static void on_click(uint gpio, uint32_t event_mask);

};

}
}
