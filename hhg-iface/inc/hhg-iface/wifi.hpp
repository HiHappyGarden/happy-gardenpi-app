/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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

    enum fsm_state
    {
        NONE                = 0x00,
        DISCONNECTED        = (1 << 0),
        WAIT_CONNECTION     = (1 << 1),
        CONNECTED           = (1 << 2),
        WAIT_IP             = (1 << 3),
        HAS_IP             =  (1 << 4)
    } fsm_state = fsm_state::NONE;

    using ptr = os::unique_ptr<wifi>;
    using on_ntp_received = void (*)(os::exit status, time_t);

    ~wifi() override = default;

    virtual os::exit connect(const os::string<32>& ssid, const os::string<64>& passwd, enum auth auth, os::error **error) OSAL_NOEXCEPT = 0;

    virtual os::exit ntp_start(on_ntp_received, os::error **error) OSAL_NOEXCEPT = 0;

    virtual os::string<15> get_ip_address_str() const OSAL_NOEXCEPT = 0;

    virtual uint32_t get_ip_address() const OSAL_NOEXCEPT = 0;

    virtual os::pair<bool, bool> is_connected() OSAL_NOEXCEPT = 0;

    virtual uint32_t get_timeout() const OSAL_NOEXCEPT = 0;

    virtual uint32_t get_fsm_state() const OSAL_NOEXCEPT  = 0;
};




}
}
