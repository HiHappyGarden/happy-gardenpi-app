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

#include "hhg-driver/rotary-encored.hpp"
#include "hhg-driver/os-config.hpp"
#include "osal/osal.hpp"

#include <pico/types.h>

namespace hhg::driver
{
inline namespace v1
{

class pico_rotary_encoder final : public rotary_encoder
{

    static inline pico_rotary_encoder* singleton = nullptr;

    bool run = true;
    os::thread polling{"rotary_encoder", hhg::driver::NORMAL, 256, pico_rotary_encoder::encoder_handle};

    event *obj = nullptr;
    event::callback callback = nullptr;
public:

    enum pin : uint
    {
        ENCODER_A   = 19,
        ENCODER_B   = 20,
        ENCODER_BTN = 21
    };

    pico_rotary_encoder() OS_NOEXCEPT;
    ~pico_rotary_encoder() OS_NOEXCEPT override;

    os::exit init(os::error** error) OS_NOEXCEPT override;

    void set_on_event(event *obj, event::callback callback) OS_NOEXCEPT override
    {
        this->obj = obj;
        this->callback = callback;
    }


private:
    static void* encoder_handle(void* arg);

};

}
}
