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

#include "hhg-iface/rotary-encored.hpp"
#include "hhg-driver/os-config.hpp"
#include "osal/osal.hpp"
#include "FreeRTOSConfig.h"

#include <pico/types.h>

namespace hhg::driver
{
inline namespace v1
{


class pico_rotary_encoder final : public hhg::iface::rotary_encoder
{

    static inline pico_rotary_encoder* singleton = nullptr;

    bool run = true;
    os::thread polling{"pico_rotary_encoder", NORMAL, 512 + 256, handler};

    event *obj = nullptr;
    event::callback callback = nullptr;
public:

    enum pin : uint
    {
        ENCODER_A   = 21,
        ENCODER_B   = 20,
        ENCODER_BTN = 19
    };

    pico_rotary_encoder() OSAL_NOEXCEPT;
    ~pico_rotary_encoder() OSAL_NOEXCEPT override;
    OSAL_NO_COPY_NO_MOVE(pico_rotary_encoder)

    os::exit init(os::error** error) OSAL_NOEXCEPT override;

    void set_on_rotary_encoder_event(event *obj, event::callback callback) OSAL_NOEXCEPT override
    {
        this->obj = obj;
        this->callback = callback;
    }


private:
    static void* handler(void* arg);

};

}
}
