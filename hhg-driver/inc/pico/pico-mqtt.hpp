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
    os::error **error = nullptr;
    mqtt_client_t *mqtt_client = nullptr;
public:
    explicit pico_mqtt(os::error **error) OSAL_NOEXCEPT;
    ~pico_mqtt() OSAL_NOEXCEPT;

    os::exit connect(const os::string<64>& client_id, const os::string<64>& broker, uint16_t port, uint8_t qos) OSAL_NOEXCEPT;

    void disconnect() OSAL_NOEXCEPT;
};


}
}



