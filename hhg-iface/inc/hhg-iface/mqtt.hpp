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

#include "hhg-iface/initializable.hpp"

namespace hhg::iface
{
inline namespace v1
{

struct mqtt : public initializable
{
    enum QOS
    {
        QOS_0 = 0, // At most once
        QOS_1 = 1, // At least once
        QOS_2 = 2  // Exactly once
    } ;

    struct receive
    {
        virtual ~receive() = default;

        virtual void on_receive(mqtt mqtt, const uint8_t data[], size_t size) const OSAL_NOEXCEPT = 0;
    };

    using on_receive = void (receive::*)(mqtt mqtt, const uint8_t data[], size_t size) const OSAL_NOEXCEPT;

    using on_changed_connection = void (*)(os::exit, uint16_t exit_code) OSAL_NOEXCEPT;

    using ptr = os::unique_ptr<hhg::iface::mqtt>;

    ~mqtt() OSAL_NOEXCEPT = default;

    virtual os::exit connect(const char client_id[], const char broker[], uint16_t port, QOS qos, on_changed_connection on_changed_connection) OSAL_NOEXCEPT = 0;

    virtual void disconnect() OSAL_NOEXCEPT = 0;

    virtual bool is_connected() const OSAL_NOEXCEPT = 0;

    virtual os::exit unsubscribe (const char topic[]) OSAL_NOEXCEPT = 0;

    virtual os::exit subscribe (const char topic[], const receive* on_receive, hhg::iface::mqtt::on_receive on_receive_callback) OSAL_NOEXCEPT = 0;

};


}
}