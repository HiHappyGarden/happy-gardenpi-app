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

namespace hhg::app
{
inline namespace v1
{

namespace
{
    constexpr char APP_TAG[] = "APP MQTT";
    constexpr uint8_t MAX_ERROR = 5;
}

void* app_mqtt::handle(void*)
{
    if(singleton == nullptr)
    {
        return nullptr;
    }

    const uint8_t FSM_SLEEP = 100;

    time_t generic_timer = 0;
    enum fsm_state fsm_state_old = NONE;
    uint8_t error = 0;
    
    while(singleton)
    {
        switch(singleton->fsm_state)
        {
            case NONE:
                OSAL_LOG_INFO(APP_TAG, "NONE");
                fsm_state_old = NONE;
                break;
            case DISCONNECTED:
                singleton->events.clear(0xFFFF);
                singleton->events.set(DISCONNECTED);

                OSAL_LOG_INFO(APP_TAG, "DISCONNECTED");
                osal_us_sleep(1_s);

                error = 0;
                fsm_state_old = DISCONNECTED;
                singleton->fsm_state = WAIT_CONNECTION;
                break;
            case WAIT_CONNECTION:
            {
                if(singleton->app_main.get_fsm_state() & app_main::READY)
                {

                    singleton->app_config.get_mqtt_broker();

                    if(singleton->mqtt->connect(
                            singleton->app_config.get_serial()
                            , singleton->app_config.get_mqtt_broker()
                            , singleton->app_config.get_mqtt_port()
                            , 1
                            ) == exit::OK)
                    {
                        error = 0;
                        fsm_state_old = WAIT_CONNECTION;
                        singleton->fsm_state = CONNECTED;
                    }
                    else
                    {
                        if(error)
                        {
                            os::printf_stack_error(APP_TAG, *singleton->error);
                            delete *singleton->error;
                            *singleton->error = nullptr;
                        }

                        fsm_state_old = WAIT_CONNECTION;
                        singleton->fsm_state = ERROR;
                    }
                }
                else
                {
                    singleton->events.set(WAIT_CONNECTION);
                }
                break;
            }
            case CONNECTED:
            {
                if( !(singleton->app_main.get_fsm_state() & app_main::READY) )
                {
                    error = 0;
                    singleton->fsm_state = DISCONNECTED;
                    fsm_state_old = CONNECTED;
                    break;
                }

                if(singleton->mqtt->is_connected())
                {
                    singleton->events.clear(WAIT_CONNECTION);
                    singleton->events.set(CONNECTED);

                    if(generic_timer <= 0)
                    {
                        generic_timer = app_main::ONE_SEC_IN_MILLIS;
                        OSAL_LOG_DEBUG(APP_TAG, "CONNECTED");
                    }
                    else
                    {
                        generic_timer -= FSM_SLEEP;
                    }
                }
                else
                {
                    error = 0;
                    fsm_state_old = CONNECTED;
                    singleton->fsm_state = DISCONNECTED;
                }

                error = 0;
                fsm_state_old = CONNECTED;
                singleton->fsm_state = WAIT_REGISTER_SUBSCRIPTION;
                break;
            }
            case WAIT_REGISTER_SUBSCRIPTION:
            {
                singleton->events.set(WAIT_REGISTER_SUBSCRIPTION);

                error = 0;
                fsm_state_old = WAIT_REGISTER_SUBSCRIPTION;
                singleton->fsm_state = REGISTERED_SUBSCRIPTION;
                break;
            }
            case REGISTERED_SUBSCRIPTION:
            {
                singleton->events.clear(WAIT_REGISTER_SUBSCRIPTION);
                singleton->events.set(REGISTERED_SUBSCRIPTION);

                error = 0;
                fsm_state_old = REGISTERED_SUBSCRIPTION;
                singleton->fsm_state = CONNECTED;
                break;
            }
            case ERROR:
                OSAL_LOG_WARNING(APP_TAG, "ERROR");

                if(error < MAX_ERROR)
                {
                    osal_us_sleep(1_s);
                    singleton->fsm_state  = fsm_state_old;
                    error++;
                }
                else
                {
                    singleton->fsm_state  = DISCONNECTED;
                    error = 0;
                }


                break;
        }

        osal_us_sleep(ms_to_us(FSM_SLEEP));
    }

    if(singleton)
    {
        singleton->thread.exit();
    }


    return nullptr;
}


app_mqtt::app_mqtt(const class app_main& app_main, const hhg::iface::wifi::ptr& wifi, const hhg::iface::mqtt::ptr& mqtt, const class app_config& app_config, const hhg::app::app_parser& app_parser) OSAL_NOEXCEPT
       : app_main(app_main)
       , wifi(wifi)
       , mqtt(const_cast<mqtt::ptr&>(mqtt))
       , app_parser(app_parser)
       , app_config(app_config)
{

}
app_mqtt::~app_mqtt() = default;

os::exit app_mqtt::init(class error** error) OSAL_NOEXCEPT
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
    this->error = error;

    start();

    return thread.create();
}

void app_mqtt::start() OSAL_NOEXCEPT
{
    fsm_state = DISCONNECTED;
}


}
}