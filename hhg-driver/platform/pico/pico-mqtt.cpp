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

#include "pico/pico-mqtt.hpp"
#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif
#ifdef MQTT_CA_CRT
#include "ca-crt.hpp"
#endif
#ifdef MQTT_CLIENT_CRT
#include "client-crt.hpp"
#endif
#ifdef MQTT_CLIENT_KEY
#include "client-key.hpp"
#endif
using namespace os;
using hhg::iface::mqtt;

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <lwip/pbuf.h>
#include <lwip/tcp.h>
#include <lwip/dns.h>
#include <lwip/altcp_tcp.h>
#include <lwip/altcp_tls.h>
#include <lwip/apps/mqtt_priv.h>

#include <string.h>

namespace hhg::driver
{
inline namespace v1
{

namespace
{
constexpr char APP_TAG[] = "DRV MQTT";
}


pico_mqtt::pico_mqtt(os::error **error) OSAL_NOEXCEPT
: error(error)
{
    memset(subscriptions, 0, sizeof(subscriptions));
}

pico_mqtt::~pico_mqtt() OSAL_NOEXCEPT
{
    if(mqtt_client)
    {
        mqtt_client_free(mqtt_client);
        mqtt_client = nullptr;
    }
}

os::exit pico_mqtt::init(os::error** error) OSAL_NOEXCEPT
{
    mqtt_client = mqtt_client_new();

    if(mqtt_client == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("No memory for mqtt_client", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return exit::KO;
    }

    return exit::OK;
}


os::exit pico_mqtt::connect(const char client_id[], const char broker[], uint16_t port, uint8_t qos) OSAL_NOEXCEPT
{
    if(strlen(client_id) == 0 || strlen(broker) == 0)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("Some param are empty", error_type::OS_EINVAL);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return os::exit::KO;
    }

#if HHG_WIFI_DISABLE == 0
OSAL_LOG_ERROR(APP_TAG, "---->1.1");
    mqtt_connect_client_info_t ci
    {
            .client_id = client_id,
            .keep_alive = 0,
            .will_topic = nullptr,
            .will_msg = nullptr,
            .will_qos = qos,
            .will_retain = 0
    };
OSAL_LOG_ERROR(APP_TAG, "---->1.2");
    struct altcp_tls_config *tls_config{};
OSAL_LOG_ERROR(APP_TAG, "---->1.3");
#if defined(MQTT_CA_CRT) && defined(MQTT_CLIENT_CRT) && defined(MQTT_CLIENT_KEY)
    OSAL_LOG_INFO(APP_TAG, "Setting up TLS with 2wayauth");
    tls_config = altcp_tls_create_config_client_2wayauth(
            reinterpret_cast<const u8_t*>(CA_CRT), 1 + strlen(CA_CRT),
            reinterpret_cast<const u8_t*>(CLIENT_KEY), 1 + strlen(CLIENT_KEY),
            reinterpret_cast<const u8_t*>(""), 0,
            reinterpret_cast<const u8_t*>(CLIENT_CRT), 1 + strlen(CLIENT_CRT)
    );
    OSAL_LOG_ERROR(APP_TAG, "---->1.4");
#endif

    OSAL_LOG_ERROR(APP_TAG, "---->1.5");
    if (tls_config == nullptr)
    {
        OSAL_LOG_ERROR(APP_TAG, "Failed to initialize config\n");
        return os::exit::KO;
    }

    ci.tls_config = tls_config;
#endif
    return os::exit::OK;
}

void pico_mqtt::disconnect() OSAL_NOEXCEPT
{

}

bool pico_mqtt::is_connected() const OSAL_NOEXCEPT
{

    return true;
}

os::exit pico_mqtt::unsubscribe(const char topic[])
{
    return exit::KO;
}

os::exit pico_mqtt::subscribe(const char topic[], const receive* on_receive, mqtt::on_receive on_receive_callback)
{


    return exit::KO;
}

}
}
