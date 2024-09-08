/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-app/app-main.hpp"

using hhg::iface::time;
using hhg::iface::wifi;
using hhg::driver::hardware;
using hhg::iface::io_source;
using hhg::iface::io;
using namespace os;

#include "hhg-app/app-parser-commands.hpp"

namespace hhg::app
{
inline namespace v1
{

namespace
{
constexpr const char APP_TAG[] = "APP MAIN";

}

void *app_main::handler(void *arg)
{
    if(arg == nullptr)
    {
        singleton->fsm.run = false;
        OSAL_LOG_INFO(APP_TAG, "Arg nullptr");
    }

    auto error = static_cast<os::error **>(arg);
    time_t &&now_in_millis = singleton->hardware.get_time()->get_timestamp(0, false, error) * ONE_SEC_IN_MILLIS;

    time_t generic_timer = 0;
    time_t sync_timestamp_timer = 0;

    singleton->app_led.loading();

    while(singleton->fsm.run)
    {
        switch(singleton->fsm.state)
        {
            case CHECK_USERS:
            {
                singleton->fsm.events.clear(ERROR);

                auto admin = singleton->app_config.get_user(app_config::user::ADMIN);
                auto user = singleton->app_config.get_user(1);
                if(
                        admin.user.length() && admin.passwd.length()
                        && user.user.length() && user.passwd.length()
                        )
                {
                    singleton->app_led.loading();
                    singleton->fsm.errors = 0;
                    singleton->fsm.events.set(CHECK_USERS);

                    OSAL_LOG_INFO(APP_TAG, "User OK state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = CHECK_WIFI_ENABLED;
                    singleton->fsm.old_state = CHECK_USERS;
                }
                else
                {
                    now_in_millis = singleton->hardware.get_time()->get_timestamp(0, false, error) * ONE_SEC_IN_MILLIS;
                    if(error && *error)
                    {
                        printf_stack_error(APP_TAG, *error);
                        delete *error;
                        *error = nullptr;
                        singleton->handle_error();
                    }
                    if(generic_timer == 0)
                    {
                        OSAL_LOG_WARNING(APP_TAG, "Waiting to set users");
                        generic_timer = ONE_SEC_IN_MILLIS;
                        singleton->app_led.warning();
                    }
                    else
                    {
                        generic_timer -= FSM_SLEEP;
                    }
                }
                break;
            }
            case CHECK_WIFI_ENABLED:
            {
                if(!singleton->app_config.is_wifi_enabled())
                {
                    singleton->app_led.loading();
                    singleton->fsm.errors = 0;
                    singleton->fsm.events.set(CHECK_WIFI_ENABLED);

                    OSAL_LOG_INFO(APP_TAG, "WIFI disabled state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = CHECK_TIMESTAMP;
                    singleton->fsm.old_state = CHECK_WIFI_ENABLED;
                }
                else
                {
                    OSAL_LOG_INFO(APP_TAG, "WIFI enabled state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = CHECK_WIFI_PARAMS;
                    singleton->fsm.old_state = CHECK_WIFI_ENABLED;
                }
                break;
            }
            case CHECK_WIFI_PARAMS:
            {
                auto &&ssid = singleton->app_config.get_wifi_ssid();
                auto &&passwd = singleton->app_config.get_wifi_passwd();
                auto auth = singleton->app_config.get_wifi_auth();
                if(ssid.length() && passwd.length() && auth)
                {
                    singleton->fsm.events.clear(CHECK_WIFI_WAIT_PARAMS);
                    singleton->fsm.events.set(CHECK_WIFI_PARAMS);

                    if(singleton->hardware.get_wifi()->connect(ssid, passwd, wifi::auth{auth}, error) == exit::OK)
                    {
                        singleton->app_led.loading();
                        singleton->fsm.errors = 0;
                        singleton->fsm.events.set(CHECK_WIFI_WAIT_CONNECTION);

                        OSAL_LOG_INFO(APP_TAG, "WIFI params state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                        singleton->fsm.state = CHECK_WIFI_WAIT_CONNECTION;
                        singleton->fsm.old_state = CHECK_WIFI_PARAMS;
                    }
                    else
                    {
                        singleton->handle_error();
                        if(error && *error)
                        {
                            printf_stack_error(APP_TAG, *error);
                            delete *error;
                            *error = nullptr;
                        }

                    }

                }
                else
                {
                    singleton->fsm.events.set(CHECK_WIFI_WAIT_PARAMS);

                    OSAL_LOG_INFO(APP_TAG, "WIFI params state:%s - KO", state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = CHECK_WIFI_WAIT_PARAMS;
                    singleton->fsm.old_state = CHECK_WIFI_PARAMS;
                }

                break;
            }
            case CHECK_WIFI_WAIT_PARAMS:
            {
                now_in_millis = singleton->hardware.get_time()->get_timestamp(0, false, error) * ONE_SEC_IN_MILLIS;
                if(error && *error)
                {
                    printf_stack_error(APP_TAG, *error);
                    delete *error;
                    *error = nullptr;
                    singleton->handle_error();
                }

                if(generic_timer == 0)
                {
                    OSAL_LOG_WARNING(APP_TAG, "Waiting to set WIFI params");
                    generic_timer = ONE_SEC_IN_MILLIS;
                    singleton->app_led.warning();

                    singleton->fsm.state = CHECK_WIFI_PARAMS;
                    singleton->fsm.old_state = CHECK_WIFI_WAIT_PARAMS;
                }
                else
                {
                    generic_timer -= FSM_SLEEP;
                }
                break;
            }
            case CHECK_WIFI_WAIT_CONNECTION:
            {
                if(singleton->hardware.get_wifi()->is_connected())
                {
                    singleton->app_led.loading();
                    singleton->fsm.errors = 0;
                    singleton->fsm.events.clear(CHECK_WIFI_WAIT_CONNECTION);
                    singleton->fsm.events.set(CONNECTED);

                    OSAL_LOG_INFO(APP_TAG, "Connection OK state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = CHECK_TIMESTAMP;
                    singleton->fsm.old_state = CHECK_WIFI_WAIT_CONNECTION;
                }
                break;
            }
            case CHECK_TIMESTAMP:
            {
                singleton->fsm.events.clear(SINCH_TIMESTAMP);
                if(now_in_millis > TIMESTAMP_2020 * ONE_SEC_IN_MILLIS)
                {
                    auto &&date_time = singleton->hardware.get_time()->get_date_time(time::FORMAT, singleton->app_config.get_timezone(), singleton->app_config.get_daylight_saving_time(), error);
                    if(error && *error)
                    {
                        printf_stack_error(APP_TAG, *error);
                        delete *error;
                        *error = nullptr;
                        singleton->handle_error();
                    }

                    singleton->app_led.loading();
                    singleton->fsm.errors = 0;
                    singleton->fsm.events.set(CHECK_TIMESTAMP);
                    OSAL_LOG_INFO(APP_TAG, "Date time:%s state:%s - OK", date_time.c_str(), state_to_string(singleton->fsm.state).c_str());
                    singleton->fsm.state = READY;
                    singleton->fsm.old_state = CHECK_TIMESTAMP;
                    generic_timer = 0;
                    sync_timestamp_timer = ONE_HOUR_IN_MILLIS;
                }
                else
                {
                    if(generic_timer == 0)
                    {
                        OSAL_LOG_WARNING(APP_TAG, "Waiting to set timestamp");
                        generic_timer = ONE_SEC_IN_MILLIS;
                        singleton->app_led.warning();
                        singleton->fsm.state = SINCH_TIMESTAMP;
                        singleton->fsm.old_state = CHECK_TIMESTAMP;
                    }
                    else
                    {
                        generic_timer -= FSM_SLEEP;
                    }

                }
                break;
            }
            case SINCH_TIMESTAMP:
            {
                if(singleton->fsm.events.get() & CONNECTED)
                {
                    singleton->hardware.get_wifi()->ntp_start([](os::exit status, time_t timestamp)
                                                              {
                                                                  if(status == exit::OK)
                                                                  {
                                                                      singleton->hardware.get_time()->set_timestamp(timestamp, nullptr);
                                                                  }
                                                              }, nullptr);
                }

                now_in_millis = singleton->hardware.get_time()->get_timestamp(0, false, error) * ONE_SEC_IN_MILLIS;
                if(error && *error)
                {
                    printf_stack_error(APP_TAG, *error);
                    delete *error;
                    *error = nullptr;
                    singleton->handle_error();
                }

                singleton->fsm.errors = 0;
                singleton->fsm.events.set(SINCH_TIMESTAMP);
                singleton->fsm.events.clear(CHECK_TIMESTAMP);
                OSAL_LOG_INFO(APP_TAG, "state:%s - OK", state_to_string(singleton->fsm.state).c_str());
                singleton->fsm.state = CHECK_TIMESTAMP;
                singleton->fsm.old_state = SINCH_TIMESTAMP;
                break;
            }
            case READY:
            {
                if(generic_timer <= 0)
                {
                    OSAL_LOG_DEBUG(APP_TAG, "Check zones");
                    singleton->fsm.events.set(READY);
                    singleton->fsm.old_state = READY;

                    singleton->app_led.ready();

                    schedule current_schedule;
                    if(singleton->app_data.get_schedule(now_in_millis / ONE_SEC_IN_MILLIS, current_schedule))
                    {
                        OSAL_LOG_DEBUG(APP_TAG, "");
                        current_schedule.status = status::RUN;
                    }

                    if(sync_timestamp_timer == 0)
                    {
                        OSAL_LOG_WARNING(APP_TAG, "Start sinch timestamp");
                        singleton->fsm.state = SINCH_TIMESTAMP;
                        singleton->fsm.old_state = CHECK_TIMESTAMP;
                    }
                    else
                    {
                        sync_timestamp_timer -= ONE_SEC_IN_MILLIS;
                    }

                    generic_timer = ONE_SEC_IN_MILLIS;
                }
                else
                {
                    generic_timer -= FSM_SLEEP;
                }
                break;
            }
            case EXECUTE_ZONE:
            {

                break;
            }
            case ERROR:
            {
                OSAL_LOG_FATAL(APP_TAG, "To many error occurred reset state");
                singleton->fsm.events.clear( 0x03FF );
                singleton->fsm.old_state = singleton->fsm.state;
                singleton->fsm.state = CHECK_USERS;
                break;
            }
            default:
            {
                break;
            }
        }
        us_sleep(ms_to_us(FSM_SLEEP));
        now_in_millis += FSM_SLEEP;
    }

    singleton->fsm_thread.exit();
    OSAL_LOG_DEBUG(APP_TAG, "Exit thread");

    return nullptr;
}


app_main::app_main(driver::hardware &hardware, class error **error) OSAL_NOEXCEPT
        : hardware(hardware)
        , app_config(hardware.get_fs_io())
        , app_data(hardware.get_fs_io())
//        , app_parser(hardware.get_uart())
        , app_led(hardware.get_rgb_led())
        , app_display_handler(hardware.get_lcd()
                              , hardware.get_rotary_encoder()
                              , hardware.get_button()
                              , hardware.get_time()
                              , *this
                              , app_data
                              , app_config
                              , app_parser
                              )
{
    if(singleton)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return;
    }


    singleton = this;
}

app_main::~app_main() OSAL_NOEXCEPT = default;

os::exit app_main::init(class os::error **error) OSAL_NOEXCEPT
{
    OSAL_LOG_INFO(APP_TAG, "Init APP LED");
    if(app_led.init(error) == exit::KO)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("app_led.init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init APP LED - OK");

    OSAL_LOG_INFO(APP_TAG, "Init APP PARSER");
    if(app_parser.init(error) == exit::KO)
    {
        app_led.error();
        return exit::KO;
    }
    if(set_app_parser(app_parser, error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
    }
    hardware.get_uart()->set_on_receive(&app_parser, &io::receive::on_receive);
    app_parser.register_io(io_source::UART, hardware.get_uart().get());
    OSAL_LOG_INFO(APP_TAG, "Init APP PARSER - OK");

    OSAL_LOG_INFO(APP_TAG, "Init APP CONFIG");
    if(app_config.init(error) == exit::KO)
    {
        if(error && *error)
        {
            printf_stack_error(APP_TAG, *error);
            delete (*error);
            *error = nullptr;
        }

        OSAL_LOG_WARNING(APP_TAG, "Load default config");
        if(app_config.load_default(error) == exit::KO)
        {
            if(error && *error)
            {
                *error = OSAL_ERROR_APPEND(*error, "Load default config fail", error_type::OS_ENOENT);
                OSAL_ERROR_PTR_SET_POSITION(*error);
            }
            app_led.error();
            return exit::KO;
        }
        OSAL_LOG_INFO(APP_TAG, "Load default config - OK");
    }
    if(set_app_config(app_config, error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init APP CONFIG - OK");

    OSAL_LOG_INFO(APP_TAG, "Check serial");
    if(strncmp(app_config.get_serial(), "", 16) == 0)
    {
        app_config.set_serial(hardware::get_serial().c_str());
        if(app_config.store(error) == exit::KO)
        {
            if(error && *error)
            {
                *error = OSAL_ERROR_BUILD("app_config.set_serial() impossible store config.", error_type::OS_EFAULT);
                OSAL_ERROR_PTR_SET_POSITION(*error);
            }
            app_led.error();
            return exit::KO;
        }
        OSAL_LOG_INFO(APP_TAG, "Update serial");
    }

    OSAL_LOG_INFO(APP_TAG, "Init APP DATA");
    if(app_data.init(error) == exit::KO)
    {
        if(error && *error)
        {
            printf_stack_error(APP_TAG, *error);
            delete (*error);
            *error = nullptr;
        }
        OSAL_LOG_WARNING(APP_TAG, "Impossible load data, reset");
        app_data.reset();

        OSAL_LOG_WARNING(APP_TAG, "Store default data");
        if(app_data.store(error) == exit::KO)
        {
            if(error)
            {
                *error = OSAL_ERROR_BUILD("app_data.store() fail.", error_type::OS_EFAULT);
                OSAL_ERROR_PTR_SET_POSITION(*error);
            }
            app_led.error();
            return exit::KO;
        }
        OSAL_LOG_INFO(APP_TAG, "Store default data - OK");
    }
    if(set_app_data(app_data, error) == exit::KO)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("set_app_data() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init APP DATA - OK");

    OSAL_LOG_INFO(APP_TAG, "Init APP DISPLAY HANDLER");
    if(app_display_handler.init(error) == exit::KO)
    {
        app_led.error();
        return exit::KO;
    }
    set_app_display_handler(app_display_handler);
    app_display_handler.set_on_receive(&app_parser, &io::receive::on_receive);
    app_parser.register_io(io_source::DISPLAY, &app_display_handler);
    app_parser.set_on_logout(&app_display_handler, &hhg::app::app_parser::auth::on_logout);
    OSAL_LOG_INFO(APP_TAG, "Init APP DISPLAY HANDLER - OK");

    OSAL_LOG_INFO(APP_TAG, "Set timer to parser");
    if(set_time(const_cast<class time *>(hardware.get_time().get()), error) == exit::KO)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("set_time() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
    }

    auto config_data = app_config.get_config(false);

    OSAL_LOG_INFO(APP_TAG, "Show configuration");
    OSAL_LOG_INFO(APP_TAG, "\r\n%s", config_data);

    delete config_data;

    return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error **error) OSAL_NOEXCEPT
{
    return fsm_thread.create(error, error);
}

uint32_t app_main::get_state() const
{
    return fsm.events.get();
}

string<32> app_main::state_to_string(app_main::state state)
{
    switch(state)
    {
        case app_main::CHECK_USERS:
            return {"CHECK_USERS"};
        case app_main::CHECK_WIFI_ENABLED:
            return { "CHECK_WIFI_ENABLED"};
        case app_main::CHECK_WIFI_PARAMS:
            return { "CHECK_WIFI_PARAMS"};
        case app_main::CHECK_WIFI_WAIT_PARAMS:
            return { "CHECK_WIFI_WAIT_PARAMS"};
        case app_main::CHECK_WIFI_WAIT_CONNECTION:
            return { "CHECK_WIFI_WAIT_CONNECTION"};
        case app_main::CONNECTED:
            return { "CONNECTED"};
        case app_main::CHECK_TIMESTAMP:
            return { "NONE"};
        case app_main::SINCH_TIMESTAMP:
            return { "SINCH_TIMESTAMP"};
        case app_main::READY:
            return { "READY"};
        case app_main::EXECUTE_ZONE:
            return { "EXECUTE_ZONE"};
        case app_main::ERROR:
            return { "ERROR"};
    }
    return "";
}

os::exit app_main::handle_error() OSAL_NOEXCEPT
{
    app_led.error();
    singleton->app_led.error();
    if(fsm.errors < fsm::MAX_ERROR)
    {
        fsm.errors++;
        OSAL_LOG_ERROR(APP_TAG, "Error %u/%u on state: %s", fsm.errors, fsm::MAX_ERROR, state_to_string(fsm.state).c_str());
        us_sleep(500_ms);
        return exit::OK;
    }
    else
    {
        fsm.errors = 0;
        fsm.old_state = fsm.state;
        fsm.state = ERROR;
        return exit::KO;
    }
}


}
}

