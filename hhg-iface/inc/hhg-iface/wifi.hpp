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

namespace hhg::iface
{
inline namespace v1
{

struct wifi : public initializable
{

    enum class auth : uint32_t
    {
        OPEN = 1,
        WPA_TKIP_PSK,
        WPA2_AES_PSK,
        WPA2_MIXED_PSK
    };

    struct on_connection_event
    {
        using callback = void(on_connection_event::*)(bool old_connected, bool new_connected);

        virtual ~on_connection_event() = default;
        virtual void on_change_connection(bool old_connected, bool new_connected) OS_NOEXCEPT = 0;
    };


    using ptr = os::unique_ptr<wifi>;
    using on_ntp_received = void (*)(os::exit status, time_t);

    ~wifi() override = default;

    virtual os::exit connect(const os::string<32>& ssid, const os::string<64>& passwd, enum auth auth, os::error **error) const OS_NOEXCEPT = 0;

    virtual void set_change_connection(on_connection_event* obj, on_connection_event::callback callback) OS_NOEXCEPT = 0;

    virtual os::exit ntp_start(on_ntp_received, os::error **error) OS_NOEXCEPT = 0;

};




}
}
