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

#include "hhg-iface/button.hpp"

#include <pico/types.h>
#include <pico/time.h>

namespace hhg::driver
{
inline namespace v1
{

//TODO: implement REAL singleton
class pico_button : public hhg::iface::button
{
    event *obj = nullptr;
    hhg::iface::button::event::callback callback = nullptr;

    static inline pico_button* singleton = nullptr;
public:

    enum pin : uint
    {
        PIN = 22

    };

    pico_button();
    ~pico_button() override;
    OS_NO_COPY_NO_MOVE(pico_button);

    inline void set_on_button_click(event *obj, event::callback callback) OS_NOEXCEPT override
    {
        this->obj = obj;
        this->callback = callback;
    }

    os::exit init(os::error **error) OS_NOEXCEPT override;

private:
    static inline bool fall = false;
    static inline alarm_id_t alarm_id = 0;
    static void on_click(uint gpio, uint32_t event_mask);

};

}
}
