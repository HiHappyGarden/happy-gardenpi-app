/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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



#include "hhg-app/app-parser-commands.hpp"
#include "hhg-app/app-parser.hpp"
#include "hhg-app/app-config.hpp"
#include "hhg-app/app-data.hpp"
#include "hhg-app/app-display-handler.hpp"
#include "hhg-iface/time.hpp"
using hhg::iface::time;
using hhg::iface::io_source;
using namespace hhg::driver;

#if defined(HHG_USER) && defined(HHG_PASSWD)
#define ACCESS_ALL_USERS HHG_ADMIN_USER HHG_DIVISOR HHG_USER
#else
#define ACCESS_ALL_USERS HHG_ADMIN_USER
#endif

#include <errno.h>
#include <stdint.h>

using namespace os;

namespace hhg::app
{
inline namespace v1
{

static class app_config* app_config = nullptr;
static class app_data* app_data = nullptr;
static struct time* time = nullptr;
static class app_parser* app_parser = nullptr;
static class parser* parser = nullptr;
static class app_display_handler* app_display_handler = nullptr;


namespace
{



entry commands_rtc[] =
{
	{.key = "1", .custom_func = [](auto data, auto entry, auto error)
	{
		auto t = time->get_timestamp(0, false, error);
		sprintf(data.ret_buffer,  TIME_T_STR, t);

		return exit::OK;
	}
	, .description = "Get RTC"},
	{.key = "2", .custom_func = [](auto data, auto entry, auto error)
	{

		char* ptr = nullptr;
		time_t t = strtoll(data.tokens[2].start, &ptr, 10);
		if (ptr == data.tokens[2].start || *ptr != '\0' || ((t== LONG_MIN || t == LONG_MAX) && errno == ERANGE))
		{
	        if(error)
	        {
	            *error = OSAL_ERROR_BUILD("Convention string to time_t fail", error_type::OS_EINVAL);
	            OSAL_ERROR_PTR_SET_POSITION(*error);
	        }
			return exit::KO;
		}

		time->set_timestamp(t, error);


		snprintf(data.ret_buffer, data.ret_buffer_len, "%s", hhg::parser::parser::OK);

		return exit::OK;
	}
	, .description = "Set RTC", .access = ACCESS_ALL_USERS},
	{.key = "3", .custom_func = [](auto data, auto entry, auto error)
	{
		auto&& date_time = time->get_date_time(time::FORMAT, 0, false, error);
		strncpy(data.ret_buffer, date_time.c_str(), data.ret_buffer_len);

		return exit::OK;
	}
	, .description = "Get RTC readable format"}
};
constexpr const size_t commands_rtc_size = sizeof(commands_rtc) / sizeof(commands_rtc[0]);


entry commands_config[] =
{
	{.key = "1", .description = "Get serial"},
	{.key = "2", .description = "Set serial", .access = ACCESS_ALL_USERS},
	{.key = "3", .description = "Get description"},
	{.key = "4", .description = "Set description", .access = ACCESS_ALL_USERS},
	{.key = "5", .description = "Get zones size"},
    {.key = "6", .description = "Set wifi ssid", .access = ACCESS_ALL_USERS},
    {.key = "7", .description = "Set wifi password", .access = ACCESS_ALL_USERS},
    {.key = "8", .description = "Set wifi auth", .access = ACCESS_ALL_USERS},
    {.key = "9", .description = "Is wifi enabled"},
    {.key = "10", .description = "Set wifi enabled", .access = ACCESS_ALL_USERS},
    {.key = "11",  .custom_func = [](auto data, auto entry, auto error)
    {

        char* end_ptr = nullptr;
        uint8_t idx = strtol(data.tokens[2].start, &end_ptr, 10);
        if (end_ptr == data.tokens[2].start || *end_ptr != '\0' || errno != 0)
        {
            printf("KO");
            return exit::KO;
        }

        if(app_config->set_user(idx, data.tokens[3].start, data.tokens[4].start) == exit::KO)
        {
            printf("KO");
            return exit::KO;
        }

        printf("OK");
        return exit::OK;
    }
    ,.description = "Set/update user", .access = HHG_USER},
    {.key = "12", .description = "Set timezone", .access = ACCESS_ALL_USERS},
    {.key = "13", .description = "Set daylight saving time", .access = ACCESS_ALL_USERS},
    {.key = "CLEAR", .custom_func =  [](auto data, auto entry, auto error) -> os::exit
    {
        auto ret = app_config->clear(error);
        if(ret == exit::OK)
        {
            strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
        }
        else
        {
            strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
        }
        return ret;
    }, .description = "Clear all", .access = ACCESS_ALL_USERS},
	{.key = "STORE", .custom_func = [](auto data, auto entry, auto error)
	{
		auto ret = app_config->store(error);
		if(ret == exit::OK)
		{
			strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
		}
		else
		{
			strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
		}
		return ret;
	}
	, .description = "Store config", .access = ACCESS_ALL_USERS},

};
constexpr const size_t commands_config_size = sizeof(commands_config) / sizeof(commands_config[0]);

entry commands_data[] =
{
	{.key = "1",
	.custom_func = [](auto data, auto entry, auto error)
	{
		char* ptr = nullptr;
		uint8_t id = strtol(data.tokens[2].start, &ptr, 10);
		if (ptr == data.tokens[2].start || *ptr != '\0' || ((id == 0 || id == UINT8_MAX) && errno == ERANGE))
		{
			if(error)
			{
				*error = OSAL_ERROR_BUILD("Convention string to time_t fail", error_type::OS_EINVAL);
				OSAL_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}


		auto ret = app_data->get_schedule(id);
		if(ret)
		{
			strncpy(data.ret_buffer, ret, data.ret_buffer_len);
			delete[] ret;
			return exit::OK;
		}
		else
		{
			strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
			delete[] ret;
			return exit::KO;
		}
	},
	.description = "Get scheduler"},
	{.key = "2", .description = "Set scheduler", .access = ACCESS_ALL_USERS},
	{.key = "3",
	.custom_func = [](auto data, auto entry, auto error)
	{
		char* ptr = nullptr;
		uint8_t id_schedule = strtol(data.tokens[2].start, &ptr, 10);
		if (ptr == data.tokens[2].start || *ptr != '\0' || ((id_schedule == 0 || id_schedule == UINT8_MAX) && errno == ERANGE))
		{
			if(error)
			{
				*error = OSAL_ERROR_BUILD("Convention string to time_t fail", error_type::OS_EINVAL);
				OSAL_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}

		ptr = nullptr;
		uint8_t id = strtol(data.tokens[3].start, &ptr, 10);
		if (ptr == data.tokens[3].start || *ptr != '\0' || ((id == 0 || id == UINT8_MAX) && errno == ERANGE))
		{
			if(error)
			{
				*error = OSAL_ERROR_BUILD("Convention string to time_t fail", error_type::OS_EINVAL);
				OSAL_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}

		auto ret = app_data->get_zone(id_schedule, id);
		if(ret)
		{
			strncpy(data.ret_buffer, ret, data.ret_buffer_len);
			delete[] ret;
			return exit::OK;
		}
		else
		{
			strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
			return exit::KO;
		}

	},
    .description = "Get zone"},
	{.key = "4", .description = "Set zone", .access = ACCESS_ALL_USERS},
    {.key = "5", .custom_func = [](auto data, auto entry, auto error)
        {
            //todo: end to develop manual irrigation
            strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
            return exit::KO;
        }
        , .description = "Start manual zone", .access = ACCESS_ALL_USERS},
    {.key = "CLEAR", .custom_func =  [](auto data, auto entry, auto error) -> os::exit
        {
            auto ret = app_data->clear(error);
            if(ret == exit::OK)
            {
                strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
            }
            else
            {
                strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
            }
            return ret;
        }, .description = "Clear all zone", .access = ACCESS_ALL_USERS},
	{.key = "STORE", .custom_func = [](auto data, auto entry, auto error)
	{
		auto ret = app_data->store(error);
		if(ret == exit::OK)
		{
			strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
		}
		else
		{
			strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
		}
		return ret;
	}
	, .description = "Store data", .access = ACCESS_ALL_USERS},

};
constexpr const size_t commands_data_size = sizeof(commands_data) / sizeof(commands_data[0]);

entry commands_log[] =
{
		{.key = "1", .func = new function{set_enable_log}, .description = "Set enable log"},
		{.key = "2", .func = new function{set_level_log}, .description = "Set log level"}
};
constexpr const size_t commands_log_size = sizeof(commands_log) / sizeof(commands_log[0]);

entry commands[] =
{
	{.key = "$VER", .description = "Get version"},
	{.key = "$RTC", .next = commands_rtc, .next_size = commands_rtc_size, .description = "Rtc menu"},
	{.key = "$CONF", .next = commands_config, .next_size = commands_config_size, .description = "Configuration menu"},
	{.key = "$DATA", .next = commands_data, .next_size = commands_data_size, .description = "Data menu"},
	{.key = "$LOG", .next = commands_log, .next_size = commands_log_size, .description = "Log menu"},
    {.key = "$AUTH", .custom_func = auth, .description = "Auth from phy"},
    {.key = "$AUTH_REMOTE",  .custom_func = auth,  .description = "Auth from remote"},
    {.key = "$AUTH_DISCONNECT", .description = "Disconnect", .access = ACCESS_ALL_USERS}
};
constexpr const size_t commands_size = sizeof(commands) / sizeof(commands[0]);

}

os::exit set_app_parser(class app_parser& app_parser, error** error) OSAL_NOEXCEPT
{
    hhg::app::app_parser = const_cast<class app_parser*>(&app_parser);
	parser = const_cast<class parser*>(&app_parser.get_parser());

    string<KEY_MAX> key = "$AUTH_DISCONNECT";
    if(parser->set(key.c_str(), new method(&app_parser, &app_parser::clear_user_logged), error) == exit::KO)
    {
        return exit::KO;
    }

    return exit::OK;
}


os::exit set_app_config(class app_config& app_config, error** error) OSAL_NOEXCEPT
{
	hhg::app::app_config = &app_config;

    string<KEY_MAX> key = "$VER";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::get_version), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$CONF 1";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::get_serial), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$CONF 2";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::set_serial), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$CONF 3";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::get_descr), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$CONF 4";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::set_descr), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$CONF 5";
	if(parser->set(key.c_str(), new method(&app_config, &app_config::get_zones_size), error) == exit::KO)
	{
		return exit::KO;
	}

    key = "$CONF 6";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_wifi_ssid), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 7";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_wifi_passwd), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 8";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_wifi_auth), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 9";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::is_wifi_enabled), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 10";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_wifi_enabled), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 12";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_timezone), error) == exit::KO)
    {
        return exit::KO;
    }

    key = "$CONF 13";
    if(parser->set(key.c_str(), new method(&app_config, &app_config::set_daylight_saving_time), error) == exit::KO)
    {
        return exit::KO;
    }

	return exit::OK;
}

os::exit set_app_data(class app_data& app_data, error** error) OSAL_NOEXCEPT
{


	hhg::app::app_data = &app_data;

    string<KEY_MAX> key = "$DATA 1";
	if(parser->set(key.c_str(), new method<struct app_data, char*, uint8_t>(&app_data, &app_data::get_schedule), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$DATA 2";
	if(parser->set(key.c_str(), new method(&app_data, &app_data::set_schedule), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$DATA 3";
	if(parser->set(key.c_str(), new method(&app_data, &app_data::get_zone), error) == exit::KO)
	{
		return exit::KO;
	}

	key = "$DATA 4";
	if(parser->set(key.c_str(), new method(&app_data, &app_data::set_zone), error) == exit::KO)
	{
		return exit::KO;
	}

	return exit::OK;
}

void set_app_display_handler(class app_display_handler& app_display_handler) OSAL_NOEXCEPT
{
    hhg::app::app_display_handler = &app_display_handler;
}


os::exit set_time(struct time* time, error** error) OSAL_NOEXCEPT
{
	hhg::app::time = time;

    return  exit::OK;
}

os::exit auth(const cmd_data &data, const entry *entry, os::error **error) OSAL_NOEXCEPT
{
    if(data.tokens_len < 3)
    {
        strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
        return exit::KO;
    }

    string<hhg::parser::KEY_MAX> key;
    string<32> user;
    string<32> passwd;

    key += data.tokens[0].start;
    user += data.tokens[1].start;
    passwd += data.tokens[2].start;

    if(key == "$AUTH" && (app_parser::singleton->source == io_source::UART || app_parser::singleton->source == io_source::DISPLAY) )
    {
        auto [status, auth] = app_config->set_auth(user, passwd);
        if(status == exit::OK)
        {
            app_parser->set_user_logged(auth);
            if(app_parser::singleton->source == io_source::UART)
            {
                app_display_handler->lock();
            }
            strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
            return exit::OK;
        }
    }
    if(key == "$AUTH_REMOTE" && app_parser::singleton->source == io_source::WIFI)
    {
        auto [status, auth] = app_config->set_auth_remote(user, passwd);
        if(status == exit::OK)
        {
            app_parser->set_user_logged(auth);
            app_display_handler->lock();
            strncpy(data.ret_buffer, "OK", data.ret_buffer_len);
            return exit::OK;
        }
    }

    strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
    return exit::KO;
}

entry* get_commands() OSAL_NOEXCEPT
{
	return commands;
}

size_t get_commands_size() OSAL_NOEXCEPT
{
	return commands_size;
}

}
}
