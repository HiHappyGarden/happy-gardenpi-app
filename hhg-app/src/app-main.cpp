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
using on_connection_event = hhg::iface::wifi::on_connection_event;
using namespace os;

#include "hhg-app/app-parser-commands.hpp"

namespace hhg::app
{
inline namespace v1
{

namespace
{
constexpr const uint16_t ONE_SEC_IN_MILLIS = 1'000;
constexpr const char APP_TAG[] = "APP MAIN";

string<32> state_to_string(app_main::state state)
{
	string<32> ret;
	switch (state)
	{
        case app_main::CHECK_WIFI:
            ret = "CHECK_WIFI";
            break;
        case app_main::CHECK_USERS:
            ret = "CHECK_USERS";
            break;
		case app_main::CHECK_TIMESTAMP:
			ret = "NONE";
			break;
		case app_main::INIT:
			ret = "INIT";
			break;
		case app_main::READY:
			ret = "READY";
			break;
		case app_main::EXECUTE_ZONE:
			ret = "EXECUTE_ZONE";
			break;
		case app_main::SINCH_TIMESTAMP:
			ret = "SINCH_TIMESTAMP";
			break;
		case app_main::ERROR:
			ret = "ERROR";
			break;
	}
	return ret;
}

}

void* app_main::handler(void* arg)
{
	if(arg == nullptr)
	{
		singleton->fsm.run = false;
		OS_LOG_INFO(APP_TAG, "Arg nullptr");
	}

	auto error = static_cast<os::error**>(arg);
	time_t&& now_in_millis = singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;

	int32_t generic_timer = 0;

    singleton->app_display_handler.print_frame(false, {});
    singleton->app_display_handler.print_str("Loading...", 30, app_display_handler::valign::CENTER, app_display_handler::font::F8X8);
    singleton->app_display_handler.send_buffer();

	while(singleton->fsm.run)
	{
		switch (singleton->fsm.state)
		{
            case CHECK_USERS:
            {
                auto admin = singleton->app_config.get_user(app_config::user::ADMIN);
                auto user = singleton->app_config.get_user(1);
                if(
                    admin.user.length() && admin.passwd.length()
                    && user.user.length() && user.passwd.length()
                )
                {
                    singleton->fsm.events.set(static_cast<uint8_t>(CHECK_USERS));
                    OS_LOG_INFO(APP_TAG, "User OK state:%s - OK", state_to_string(singleton->fsm.state));
                    singleton->fsm.state = CHECK_WIFI;
                    singleton->fsm.old_state = CHECK_USERS;
                }
                else
                {
                    now_in_millis = singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
                    if(*error)
                    {
                        printf_stack_error(APP_TAG, *error);
                        delete *error;
                        singleton->handle_error();
                    }
                    if(generic_timer == 0)
                    {
                        OS_LOG_WARNING(APP_TAG, "Waiting to set users");
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
            case CHECK_WIFI:
            {
                auto&& ssid = singleton->app_config.get_wifi_ssid();
                auto&& passwd = singleton->app_config.get_wifi_passwd();
                auto auth = singleton->app_config.get_wifi_auth();
                bool connection_flag =  singleton->fsm.events.get() & static_cast<uint8_t>(CHECK_WIFI);

                if(!singleton->app_config.is_wifi_enabled())
                {
                    singleton->fsm.state = CHECK_TIMESTAMP;
                    singleton->fsm.old_state = CHECK_USERS;
                }
                if(singleton->fsm.waiting_connection && connection_flag)
                {
                    singleton->hardware.get_wifi()->ntp_start([](os::exit status, time_t timestamp)
                    {
                        if(status == exit::OK)
                        {
                            singleton->hardware.get_time()->set_timestamp(timestamp, nullptr);
                        }
                    }, nullptr);
                    singleton->fsm.events.set(static_cast<uint8_t>(CHECK_WIFI));
                    OS_LOG_INFO(APP_TAG, "Connection OK state:%s - OK", state_to_string(singleton->fsm.state));
                    singleton->fsm.state = CHECK_TIMESTAMP;
                    singleton->fsm.old_state = CHECK_USERS;
                }
                else if (ssid.length() && passwd.length() && auth)
                {
                    if ( !singleton->fsm.waiting_connection
                        && !connection_flag
                        && singleton->hardware.get_wifi()->connect(ssid, passwd, wifi::auth{auth}, error) == exit::OK)
                    {
                        singleton->fsm.waiting_connection = true;
                        OS_LOG_INFO(APP_TAG, "Waiting connection");
                    }
                    else
                    {
                        if(*error)
                        {
                            printf_stack_error(APP_TAG, *error);
                            delete *error;
                            singleton->handle_error();
                        }
                    }
                }
                else
                {
                    now_in_millis = singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
                    if(*error)
                    {
                        printf_stack_error(APP_TAG, *error);
                        delete *error;
                        singleton->handle_error();
                    }

                    if(generic_timer == 0)
                    {
                        OS_LOG_WARNING(APP_TAG, "Waiting to set WIFI params");
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
            case CHECK_TIMESTAMP:
			{
				if(now_in_millis > TIMESTAMP_2020 * ONE_SEC_IN_MILLIS)
				{
					auto&& date_time = singleton->hardware.get_time()->get_date_time(time::FORMAT, error);
			        if(*error)
			        {
			            printf_stack_error(APP_TAG, *error);
			            delete *error;
			            singleton->handle_error();
			        }

			        singleton->fsm.errors = 0;
					OS_LOG_INFO(APP_TAG, "Date time:%s state:%s - OK", date_time.c_str(), state_to_string(singleton->fsm.state));
					singleton->fsm.state = INIT;
					singleton->fsm.old_state = CHECK_TIMESTAMP;
				}
				else
				{
					now_in_millis = singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
			        if(*error)
			        {
			            printf_stack_error(APP_TAG, *error);
			            delete *error;
			            singleton->handle_error();
			        }

					if(generic_timer == 0)
					{
						OS_LOG_WARNING(APP_TAG, "Waiting to set timestamp");
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
			case INIT:
			{
				OS_LOG_DEBUG(APP_TAG, "HW check");
				singleton->fsm.events.set(INIT);

                singleton->app_led.ready();

				//TODO: hw check
				generic_timer = 0;

                auto status = singleton->fsm.events.get();
                const os::string<32>&& time = singleton->hardware.get_time()->get_date_time("%H:%M");
                singleton->app_display_handler.print_frame(status & CHECK_WIFI, time);
                singleton->app_display_handler.clean();
                singleton->app_display_handler.print_str("Ready", 30, app_display_handler::valign::CENTER, app_display_handler::font::F8X8);
                singleton->app_display_handler.send_buffer();

				singleton->fsm.errors = 0;
				OS_LOG_INFO(APP_TAG, "state:%s - OK", state_to_string(singleton->fsm.state));
				singleton->fsm.state = READY;
				singleton->fsm.old_state = INIT;
				break;
			}
			case READY:
			{
				if(generic_timer <= 0)
				{
					OS_LOG_DEBUG(APP_TAG, "Check zones");
					singleton->fsm.events.set(READY);
					singleton->fsm.old_state = READY;

//                    auto status = singleton->fsm.events.get();
//                    const os::string<32>&& time = singleton->hardware.get_time()->get_date_time("%H:%M");
//                    singleton->app_display_handler.print_frame(status & CHECK_WIFI, time);

					schedule current_schedule;
					if(singleton->app_data.get_schedule(now_in_millis / ONE_SEC_IN_MILLIS, current_schedule))
					{
						OS_LOG_DEBUG(APP_TAG, "");
						current_schedule.status = status::RUN;
						//TODO:
						//singleton->fsm.state = INIT;
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
			case SINCH_TIMESTAMP:
			{

				break;
			}
            case ERROR:
            {
                OS_LOG_FATAL(APP_TAG, "To many error occurred reset state");
                singleton->fsm.events.clear(CHECK_USERS|CHECK_WIFI|CHECK_TIMESTAMP|INIT|READY|SINCH_TIMESTAMP|ERROR);
                singleton->fsm.state              = state::CHECK_USERS;
                singleton->fsm.old_state 	        = state::CHECK_USERS;
                singleton->app_led.loading();
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
	OS_LOG_DEBUG(APP_TAG, "Exit thread");

	return nullptr;
}


app_main::app_main(driver::hardware& hardware, class error** error) OS_NOEXCEPT
: hardware(hardware)
, app_config(hardware.get_fs_io())
, app_data(hardware.get_fs_io())
, app_parser(hardware.get_uart())
, app_led(hardware.get_rgb_led())
, app_display_handler(hardware.get_lcd(), hardware.get_rotary_encoder(), hardware.get_button())
{
	if(singleton)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return;
	}




	singleton = this;
}

app_main::~app_main() OS_NOEXCEPT = default;

os::exit app_main::init(class os::error** error) OS_NOEXCEPT
{
	hardware.get_uart()->set_on_receive(&app_parser, &hhg::iface::io_on_receive::on_receive);

    OS_LOG_INFO(APP_TAG, "Init APP LED");
    if(app_led.init(error) == exit::KO)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("app_led.init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init APP LED - OK");

    OS_LOG_INFO(APP_TAG, "Init APP PARSER");
	if(app_parser.init(error) == exit::KO)
	{
        app_led.error();
		return exit::KO;
	}
	if(set_app_parser(app_parser, error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
    }
	OS_LOG_INFO(APP_TAG, "Init APP PARSER - OK");

	OS_LOG_INFO(APP_TAG, "Init APP CONFIG");
	if(app_config.init(error) == exit::KO)
	{
        if(error && *error)
        {
            printf_stack_error(APP_TAG, *error);
            delete (*error);
            *error = nullptr;
        }

		OS_LOG_WARNING(APP_TAG, "Load default config");
		if(app_config.load_default(error) == exit::KO)
		{
			if(error && *error)
			{
				*error = OS_ERROR_APPEND(*error, "Load default config fail", error_type::OS_ENOENT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
            app_led.error();
			return exit::KO;
		}
		OS_LOG_INFO(APP_TAG, "Load default config - OK");
	}
	if(set_app_config(app_config, error) == exit::KO)
	{
        if(error && *error)
        {
            *error = OS_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        app_led.error();
        return exit::KO;
	}
    OS_LOG_INFO(APP_TAG, "Init APP CONFIG - OK");

    OS_LOG_INFO(APP_TAG, "Check serial");
    if(strncmp(app_config.get_serial(), "", 16) == 0)
    {
        app_config.set_serial(hardware::get_serial().c_str());
        if(app_config.store(error) == exit::KO)
        {
            if(error && *error)
            {
                *error = OS_ERROR_BUILD("app_config.set_serial() impossible store config.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            app_led.error();
            return exit::KO;
        }
        OS_LOG_INFO(APP_TAG, "Update serial");
    }

	OS_LOG_INFO(APP_TAG, "Init APP DATA");
	if(app_data.init(error) == exit::KO)
	{
		if(error && *error)
		{
			printf_stack_error(APP_TAG, *error);
			delete (*error);
			*error = nullptr;
		}
		OS_LOG_WARNING(APP_TAG, "Impossible load data, reset");
		app_data.reset();

		OS_LOG_WARNING(APP_TAG, "Store default data");
		if(app_data.store(error) == exit::KO)
		{
			if(error)
			{
				*error = OS_ERROR_BUILD("app_data.store() fail.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
            app_led.error();
			return exit::KO;
		}
		OS_LOG_INFO(APP_TAG, "Store default data - OK");
	}
	if(set_app_data(app_data, error) == exit::KO)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("set_app_data() fail.", error_type::OS_EFAULT);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
        app_led.error();
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init APP DATA - OK");

    OS_LOG_INFO(APP_TAG, "Init APP DISPLAY HANDLER");
    if(app_display_handler.init(error) == exit::KO)
    {
        app_led.error();
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init APP DISPLAY HANDLER - OK");

	OS_LOG_INFO(APP_TAG, "Set timer to parser");
	set_time(const_cast<class time*>(hardware.get_time().get()));

    auto config_data = app_config.get_config(false);

    OS_LOG_INFO(APP_TAG, "Show configuration");
    OS_LOG_INFO(APP_TAG, "\r\n%s", config_data);

    delete config_data;

    hardware.get_wifi()->set_change_connection(this, &on_connection_event::on_change_connection);

	return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error** error) OS_NOEXCEPT
{
	return fsm_thread.create(error, error);
}

os::exit app_main::handle_error() OS_NOEXCEPT
{
    app_led.error();
    fsm.events.set(ERROR);
    singleton->app_led.error();
	if(fsm.errors < fsm::MAX_ERROR)
	{
		fsm.errors++;
        OS_LOG_ERROR(APP_TAG, "It's occurred an error %u/%u on state: %s", fsm.errors, fsm::MAX_ERROR, state_to_string(fsm.state));
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

void app_main::on_change_connection(bool old_connected, bool new_connected) OS_NOEXCEPT
{
    if(!old_connected && new_connected)
    {
        singleton->fsm.events.set(static_cast<uint8_t>(CHECK_WIFI));
    }
    else if(old_connected && !new_connected)
    {
        singleton->fsm.events.clear(static_cast<uint8_t>(CHECK_WIFI));
        singleton->fsm.waiting_connection = false;
    }
    else if(!old_connected && !new_connected)
    {
        handle_error();
        singleton->fsm.events.clear(static_cast<uint8_t>(CHECK_WIFI));
        singleton->fsm.waiting_connection = false;
    }
}



}
}

