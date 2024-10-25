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

#include "hhg-iface/initializable.hpp"
#include "hhg-iface/wifi.hpp"
#include "hhg-app/app-parser.hpp"
#include "hhg-driver/os-config.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_main;

class app_mqtt final : public hhg::iface::initializable
{
    static inline app_mqtt* singleton = nullptr;

    const class app_main& app_main;
    const hhg::iface::wifi::ptr& wifi;
    const hhg::app::app_parser& app_parser;

    os::event events;

    static void* handle(void* arg);
    os::thread thread{"wifi", hhg::driver::HIGH, 1'024, handle};

    enum fsm_state
    {
        NONE                        = 0x00,
        DISCONNECTED                = (1 << 0),
        WAIT_CONNECTION             = (1 << 1),
        CONNECTED                   = (1 << 2),
        WAIT_REGISTER_SUBSCRIPTION  = (1 << 3),
        REGISTERED_SUBSCRIPTION     = (1 << 4),
    } fsm_state = fsm_state::NONE;

public:
    app_mqtt(const class app_main& app_main, const hhg::iface::wifi::ptr& wifi, const hhg::app::app_parser& app_parser) OSAL_NOEXCEPT;
    ~app_mqtt() override;

    os::exit init(os::error** error) OSAL_NOEXCEPT override;

    void start() OSAL_NOEXCEPT;

};

}
} // hhg
