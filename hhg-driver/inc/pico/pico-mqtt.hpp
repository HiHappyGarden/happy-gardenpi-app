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

#include "hhg-iface/mqtt.hpp"

#include <lwip/apps/mqtt.h>

namespace hhg::driver
{
inline namespace v1
{


class pico_mqtt final : public hhg::iface::mqtt
{
    static constexpr inline const uint8_t SUBSCRIPTIONS_MAX = 1;
    static constexpr inline const uint16_t MQTT_SERVER_PORT = 8883;

    static inline pico_mqtt* singleton = nullptr;

    receive* on_receive;
    on_changed_connection on_changed_connection = nullptr;

    os::error **error = nullptr;
    mqtt_client_t *mqtt_client = nullptr;

    uint8_t subscriptions_len = 0;
    os::pair<const receive*, hhg::iface::mqtt::on_receive> subscriptions[SUBSCRIPTIONS_MAX];


public:

    pico_mqtt();
    ~pico_mqtt() OSAL_NOEXCEPT;


    os::exit init(os::error** error) OSAL_NOEXCEPT override;


    os::exit connect(const char client_id[], const char* broker, uint16_t port, QOS qos, mqtt::on_changed_connection on_changed_connection) OSAL_NOEXCEPT override;

    void disconnect() OSAL_NOEXCEPT override;

    bool is_connected() const OSAL_NOEXCEPT override;

    os::exit unsubscribe (const char topic[]) OSAL_NOEXCEPT override;

    os::exit subscribe (const char topic[], const receive* on_receive, hhg::iface::mqtt::on_receive on_receive_callback) OSAL_NOEXCEPT override;

private:
    void operator() (mqtt_client_t *client, void *arg, mqtt_connection_status_t status) OSAL_NOEXCEPT;

};


}
}



