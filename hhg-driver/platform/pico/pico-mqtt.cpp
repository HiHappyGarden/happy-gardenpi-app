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

#include "hhg-driver/os-config.hpp"
using namespace hhg::driver;

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <lwip/pbuf.h>
#include <lwip/tcp.h>
#include <lwip/dns.h>
#include <lwip/altcp_tcp.h>
#include <lwip/altcp_tls.h>
#include <lwip/apps/mqtt_priv.h>
#include <mbedtls/debug.h>
#include <mbedtls/error.h>

#include <string.h>

#include <FreeRTOS.h>
#include <task.h>

namespace hhg::driver
{
inline namespace v1
{

namespace
{
constexpr char APP_TAG[] = "DRV MQTT";
}


pico_mqtt::pico_mqtt() = default;

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
    if(singleton)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    memset(subscriptions, 0, sizeof(subscriptions));

    mqtt_client = mqtt_client_new();

    if(mqtt_client == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("No memory for mqtt_client", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return exit::KO;
    }

#ifdef MBEDTLS_DEBUG_C
    mbedtls_debug_set_threshold(4);
    //mbedtls_ssl_conf_dbg(&conf, mbedtls_debug, stdout);
#endif
    return exit::OK;
}

os::exit pico_mqtt::connect(const char client_id[], const char* broker, uint16_t port, QOS qos, mqtt::on_changed_connection on_changed_connection) OSAL_NOEXCEPT
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
    singleton->on_changed_connection = on_changed_connection;

#if HHG_WIFI_DISABLE == 0
    mqtt_connect_client_info_t ci
    {
            .client_id = client_id,
            .keep_alive = 1,
            .will_topic = nullptr,
            .will_msg = nullptr,
            .will_qos = static_cast<u8_t>(qos),
            .will_retain = 0
    };

    struct altcp_tls_config *tls_config{};
#if defined(MQTT_CA_CRT) && defined(MQTT_CLIENT_CRT) && defined(MQTT_CLIENT_KEY)
    OSAL_LOG_INFO(APP_TAG, "Setting up TLS with 2wayauth");

    tls_config = altcp_tls_create_config_client_2wayauth(
            reinterpret_cast<const u8_t*>(CA_CRT), 1 + strlen(CA_CRT),
            reinterpret_cast<const u8_t*>(CLIENT_KEY), 1 + strlen(CLIENT_KEY),
            reinterpret_cast<const u8_t*>(""), 0,
            reinterpret_cast<const u8_t*>(CLIENT_CRT), 1 + strlen(CLIENT_CRT)
    );
#endif
    if (tls_config == nullptr)
    {
        OSAL_LOG_ERROR(APP_TAG, "Failed to initialize config\n");
        if(error)
        {
            *error = OSAL_ERROR_BUILD("Failed to initialize config", error_type::OS_EPERM);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return os::exit::KO;
    }

    ci.tls_config = tls_config;
#endif

    OSAL_LOG_DEBUG(APP_TAG, "Try to connect %s to %s", client_id, broker);


    char task_table[1024];
    char current_task_name[configMAX_TASK_NAME_LEN + 1];
    size_t heap_free;
    size_t stack_free;
    print_memory_status(task_table, sizeof(task_table), current_task_name, sizeof(current_task_name), heap_free, stack_free);

    OSAL_LOG_INFO(APP_TAG,"current_task_name:%s\nheap_free:%u\nstack_free:%u\n%s", current_task_name, heap_free, stack_free, task_table);
    ip_addr_t ip_addr;
    IP4_ADDR(&ip_addr, 192, 168, 12, 250);

    mqtt_client_connect(mqtt_client, &ip_addr, MQTT_SERVER_PORT, [](mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
    {
        if(singleton->on_changed_connection)
        {
            singleton->on_changed_connection(status != 0 ? exit::KO: osal::exit::OK, status, lwip_strerr(status));
        }


        if (status == MQTT_CONNECT_ACCEPTED) {
    OSAL_LOG_INFO(APP_TAG,"mqtt_connection_cb: Successfully connected\\n");
            const char *pub_payload = "hello this is lwIP";
            err_t err;
            u8_t qos = 2;
            u8_t retain = 0;

            mqtt_publish(client, "/pippo", pub_payload, strlen(pub_payload), qos, retain, nullptr, arg);
        } else {
    OSAL_LOG_ERROR(APP_TAG,"mqtt_connection_cb: Disconnected, reason: %d\\n", status);
        }

    } , nullptr, &ci);


//    if (*err != ERR_OK) {
//        if(error)
//        {
//            *error = OSAL_ERROR_BUILD("mqtt_connect return ");
//            OSAL_ERROR_PTR_SET_POSITION(*error);
//        }
//        return os::exit::KO;
//    }

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

void pico_mqtt::operator()(mqtt_client_t* client, void* arg, mqtt_connection_status_t status)
{

}

}
}
