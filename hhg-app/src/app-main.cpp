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
        case app_main::CHECK_WIFI_CONF:
            ret = "CHECK_WIFI_CONF";
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
		case app_main::CHECK_ZONE:
			ret = "CHECK_ZONE";
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

void* fsm_thread_handler(void* arg)
{
	if(arg == nullptr)
	{
		app_main::singleton->fsm.run = false;
		OS_LOG_INFO(APP_TAG, "Arg nullptr");
	}

	error** error = static_cast<os::error**>(arg);
	time_t&& now_in_millis = app_main::singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;

	int32_t generic_timer = 0;

	while(app_main::singleton->fsm.run)
	{
		switch (app_main::singleton->fsm.state)
		{
            case app_main::CHECK_USERS:
            {
                auto admin = app_main::singleton->app_config.get_user(app_config::user::ADMIN);
                auto user = app_main::singleton->app_config.get_user(1);
                if(
                    admin.user.length() && admin.passwd.length()
                    && user.user.length() && user.passwd.length()
                )
                {
                    OS_LOG_INFO(APP_TAG, "User OK state:%s - OK", state_to_string(app_main::singleton->fsm.state));
                    app_main::singleton->fsm.state = app_main::CHECK_WIFI_CONF;
                    app_main::singleton->fsm.old_state = app_main::CHECK_USERS;
                }
                else
                {
                    now_in_millis = app_main::singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
                    if(*error)
                    {
                        os::printf_stack_error(APP_TAG, *error);
                        delete *error;
                        app_main::singleton->handle_error();
                    }

                    if(generic_timer == 0)
                    {
                        OS_LOG_WARNING(APP_TAG, "Waiting to set users");
                        generic_timer = ONE_SEC_IN_MILLIS;
                        if(app_main::singleton->hardware.get_rgb_led()->get_rgb().is_off())
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0xFF, 0x5A, 0X00);
                        }
                        else
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0x00, 0x00, 0X00);
                        }
                    }
                    else
                    {
                        generic_timer -= app_main::FSM_SLEEP;
                    }
                }
                break;
            }
            case app_main::CHECK_WIFI_CONF:
            {
                auto ssid = app_main::singleton->app_config.get_wifi_ssid();
                auto passwd = app_main::singleton->app_config.get_wifi_passwd();
                auto auth = app_main::singleton->app_config.get_wifi_auth();

                if(!app_main::singleton->app_config.is_wifi_enabled())
                {
                    app_main::singleton->fsm.state = app_main::CHECK_TIMESTAMP;
                    app_main::singleton->fsm.old_state = app_main::CHECK_USERS;
                }

                if (ssid.length() && passwd.length() && auth)
                {
                    if (app_main::singleton->hardware.get_wifi()->connect(ssid, passwd, wifi::auth{auth}, error) == exit::OK)
                    {
                        OS_LOG_INFO(APP_TAG, "Connection OK state:%s - OK", state_to_string(app_main::singleton->fsm.state));
                        app_main::singleton->fsm.state = app_main::CHECK_TIMESTAMP;
                        app_main::singleton->fsm.old_state = app_main::CHECK_USERS;
                    }
                    else
                    {
                        if(*error)
                        {
                            os::printf_stack_error(APP_TAG, *error);
                            delete *error;
                            app_main::singleton->handle_error();
                        }
                    }
                }
                else
                {
                    now_in_millis = app_main::singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
                    if(*error)
                    {
                        os::printf_stack_error(APP_TAG, *error);
                        delete *error;
                        app_main::singleton->handle_error();
                    }

                    if(generic_timer == 0)
                    {
                        OS_LOG_WARNING(APP_TAG, "Waiting to set WIFI params");
                        generic_timer = ONE_SEC_IN_MILLIS;
                        if(app_main::singleton->hardware.get_rgb_led()->get_rgb().is_off())
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0xFF, 0x5A, 0X00);
                        }
                        else
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0x00, 0x00, 0X00);
                        }
                    }
                    else
                    {
                        generic_timer -= app_main::FSM_SLEEP;
                    }
                }
                break;
            }
            case app_main::CHECK_TIMESTAMP:
			{
				if(now_in_millis > app_main::TIMESTAMP_2020 * ONE_SEC_IN_MILLIS)
				{
					auto&& date_time = app_main::singleton->hardware.get_time()->get_date_time(time::FORMAT, error);
			        if(*error)
			        {
			            os::printf_stack_error(APP_TAG, *error);
			            delete *error;
			            app_main::singleton->handle_error();
			        }

			        app_main::singleton->fsm.errors = 0;
					OS_LOG_INFO(APP_TAG, "Date time:%s state:%s - OK", date_time.c_str(), state_to_string(app_main::singleton->fsm.state));
					app_main::singleton->fsm.state = app_main::INIT;
					app_main::singleton->fsm.old_state = app_main::CHECK_TIMESTAMP;
				}
				else
				{
					now_in_millis = app_main::singleton->hardware.get_time()->get_timestamp(error) * ONE_SEC_IN_MILLIS;
			        if(*error)
			        {
			            os::printf_stack_error(APP_TAG, *error);
			            delete *error;
			            app_main::singleton->handle_error();
			        }

					if(generic_timer == 0)
					{
						OS_LOG_WARNING(APP_TAG, "Waiting to set timestamp");
						generic_timer = ONE_SEC_IN_MILLIS;
                        if(app_main::singleton->hardware.get_rgb_led()->get_rgb().is_off())
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0xFF, 0x5A, 0X00);
                        }
                        else
                        {
                            app_main::singleton->hardware.get_rgb_led()->set_rgb(0x00, 0x00, 0X00);
                        }
					}
					else
					{
						generic_timer -= app_main::FSM_SLEEP;
					}
				}
				break;
			}
			case app_main::INIT:
			{
				OS_LOG_DEBUG(APP_TAG, "HW check");
				app_main::singleton->fsm.events.set(app_main::INIT);

				//TODO: hw check
				generic_timer = 0;

				app_main::singleton->fsm.errors = 0;
				OS_LOG_INFO(APP_TAG, "state:%s - OK", state_to_string(app_main::singleton->fsm.state));
				app_main::singleton->fsm.state = app_main::CHECK_ZONE;
				app_main::singleton->fsm.old_state = app_main::INIT;
				break;
			}
			case app_main::CHECK_ZONE:
			{
				if(generic_timer <= 0)
				{
					OS_LOG_DEBUG(APP_TAG, "Check zones");
					app_main::singleton->fsm.events.set(app_main::CHECK_ZONE);
					app_main::singleton->fsm.old_state = app_main::CHECK_ZONE;

					schedule current_schedule;
					if(app_main::singleton->app_data.get_schedule(now_in_millis / ONE_SEC_IN_MILLIS, current_schedule))
					{
						OS_LOG_DEBUG(APP_TAG, "");
						current_schedule.status = status::RUN;
						//TODO:
						//app_main::singleton->fsm.state = app_main::INIT;
					}
					generic_timer = ONE_SEC_IN_MILLIS;
				}
				else
				{
					generic_timer -= app_main::FSM_SLEEP;
				}
				break;
			}
			case app_main::EXECUTE_ZONE:
			{

				break;
			}
			case app_main::SINCH_TIMESTAMP:
			{

				break;
			}
			default:
			{
				break;
			}
		}
		us_sleep(ms_to_us(app_main::FSM_SLEEP));
		now_in_millis += app_main::FSM_SLEEP;
	}

	app_main::singleton->fsm_thread.exit();
	OS_LOG_DEBUG(APP_TAG, "Exit thread");

	return nullptr;
}


app_main::app_main(driver::hardware& hardware, class error** error) OS_NOEXCEPT
: hardware(hardware)
, app_config(hardware.get_fs_io())
, app_data(hardware.get_fs_io())
, app_parser(hardware.get_uart())
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
	OS_LOG_INFO(APP_TAG, "Init APP PARSER");
	hardware.get_uart()->set_on_receive(&app_parser, &hhg::iface::io_on_receive::on_receive);

	if(app_parser.init(error) == exit::KO)
	{
		return exit::KO;
	}
	set_app_parser(app_parser);
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
                *error = OS_ERROR_BUILD("set_app_config() impossible store config.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
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
				*error = OS_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
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
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init APP DATA - OK");

	OS_LOG_INFO(APP_TAG, "Set timer to parser");
	set_time(const_cast<class time*>(hardware.get_time().get()));

    auto config_data = app_config.get_config(false);

    OS_LOG_INFO(APP_TAG, "Show configuration");
    OS_LOG_INFO(APP_TAG, "\r\n%s", config_data);

    delete config_data;

	return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error** error) OS_NOEXCEPT
{
	return fsm_thread.create(error, error);
}

os::exit app_main::handle_error() OS_NOEXCEPT
{
	if(fsm.errors < fsm::MAX_ERROR)
	{
		fsm.errors++;
		return exit::OK;
	}
	else
	{
		fsm.old_state = fsm.state;
		fsm.state = ERROR;
		return exit::KO;
	}
}

}
}

