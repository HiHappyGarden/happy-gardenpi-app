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

#include "hhg-app/app-mqtt.hpp"
#include "hhg-app/app-main.hpp"
using namespace os;
using namespace hhg::iface;

#include <string.h>
#include <time.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"

#include "lwip/altcp_tcp.h"
#include "lwip/altcp_tls.h"
#include "lwip/apps/mqtt.h"

#include "lwip/apps/mqtt_priv.h"

namespace hhg::app
{
inline namespace v1
{

namespace
{
    constexpr char APP_TAG[] = "APP MQTT";
}

void* app_mqtt::handle(void*)
{
    if(singleton == nullptr)
    {
        return nullptr;
    }

    const uint8_t TICK = 100;

    while(singleton)
    {
        switch(singleton->fsm_state)
        {
            case fsm_state::NONE:
                OSAL_LOG_INFO(APP_TAG, "NONE");
                break;
            case fsm_state::DISCONNECTED:
                singleton->events.clear(0xFFFF);
                singleton->events.set(fsm_state::DISCONNECTED);

                OSAL_LOG_INFO(APP_TAG, "DISCONNECTED");
                osal_us_sleep(1'000_ms);

                singleton->fsm_state = fsm_state::WAIT_CONNECTION;
                break;
            case fsm_state::WAIT_CONNECTION:
            {
                if(singleton->app_main.get_fsm_state() & app_main::READY)
                {

                    singleton->app_config.get_mqtt_broker();

                    if(singleton->connect(
                            singleton->app_config.get_mqtt_broker()
                            , singleton->app_config.get_mqtt_port()
                            , singleton->app_config.get_mqtt_subscription_topic()
                            ) == exit::OK)
                    {
                        singleton->fsm_state = fsm_state::CONNECTED;
                    }

                }
                else
                {
                    singleton->events.set(fsm_state::WAIT_CONNECTION);
                }
                break;
            }
            case fsm_state::CONNECTED:
            {
                if( !(singleton->app_main.get_fsm_state() & app_main::READY) )
                {
                    singleton->fsm_state = fsm_state::DISCONNECTED;
                    break;
                }
                singleton->events.clear(fsm_state::WAIT_CONNECTION);
                singleton->events.set(fsm_state::CONNECTED);
                OSAL_LOG_INFO(APP_TAG, "CONNECTED");

                singleton->fsm_state = fsm_state::WAIT_REGISTER_SUBSCRIPTION;

                break;
            }
            case fsm_state::WAIT_REGISTER_SUBSCRIPTION:
            {
                singleton->events.set(fsm_state::WAIT_REGISTER_SUBSCRIPTION);


                singleton->fsm_state = fsm_state::WAIT_REGISTER_SUBSCRIPTION;
                break;
            }
            case fsm_state::REGISTERED_SUBSCRIPTION:
            {
                singleton->events.clear(fsm_state::WAIT_REGISTER_SUBSCRIPTION);
                singleton->events.set(fsm_state::REGISTERED_SUBSCRIPTION);


                singleton->fsm_state = fsm_state::CONNECTED;
                break;
            }
        }

        osal_us_sleep(ms_to_us(TICK));
    }

    if(singleton)
    {
        singleton->thread.exit();
    }


    return nullptr;
}


app_mqtt::app_mqtt(const class app_main& app_main, const hhg::iface::wifi::ptr& wifi, const class app_config& app_config, const hhg::app::app_parser& app_parser) OSAL_NOEXCEPT
       : app_main(app_main)
       , wifi(wifi)
       , app_parser(app_parser)
       , app_config(app_config)
{

}
app_mqtt::~app_mqtt() = default;

os::exit app_mqtt::init(error** error) OSAL_NOEXCEPT
{
    if(singleton)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("app_mqtt::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    start();

    return thread.create();
}

void app_mqtt::start() OSAL_NOEXCEPT
{
    singleton->fsm_state = fsm_state::DISCONNECTED;
}

os::exit app_mqtt::connect(const string<64>& broker, uint16_t port, string<16> subscription_topic) OSAL_NOEXCEPT
{
    mqtt_client_t *mqtt_client = mqtt_client_new();
    return exit::OK;
}

}
}