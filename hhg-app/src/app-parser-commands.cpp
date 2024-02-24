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
#include "hhg-iface/time.hpp"
using hhg::iface::time;
using namespace hhg::driver;

#include <errno.h>

using namespace os;

namespace hhg::app
{
inline namespace v1
{

static class app_config* app_config = nullptr;
static class app_data* app_data = nullptr;
static class time* time = nullptr;
static class parser* parser = nullptr;

namespace
{



entry commands_rtc[] =
{
	{.key = "1", .custom_func = [](auto data, auto entry, auto error)
	{
		auto t = time->get_timestamp(error) + time::TIMESTAMP_2000;
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
	            *error = OS_ERROR_BUILD("Convention string to time_t fail", error_type::OS_EINVAL);
	            OS_ERROR_PTR_SET_POSITION(*error);
	        }
			return exit::KO;
		}

		t -= time::TIMESTAMP_2000;

		time->set_timestamp(t, error);


		snprintf(data.ret_buffer, data.ret_buffer_len, "%s", hhg::parser::parser::OK);

		return exit::OK;
	}
	, .description = "Set RTC"},
};
constexpr const size_t commands_rtc_size = sizeof(commands_rtc) / sizeof(commands_rtc[0]);


entry commands_config[] =
{
	{.key = "1", .description = "Get serial"},
	{.key = "2", .description = "Set serial"},
	{.key = "3", .description = "Get description"},
	{.key = "4", .description = "Set description"},
	{.key = "STORE", .custom_func = [](auto data, auto entry, auto error)
	{
		auto ret = app_config->store(error);
		if(ret == exit::OK)
		{
			strncpy(data.ret_buffer, "", data.ret_buffer_len);
		}
		else
		{
			strncpy(data.ret_buffer, "KO", data.ret_buffer_len);
		}
		return ret;
	}
	, .description = "Store config"},
};
constexpr const size_t commands_config_size = sizeof(commands_config) / sizeof(commands_config[0]);

entry commands_user[] =
{

};
constexpr const size_t commands_user_size = sizeof(commands_user) / sizeof(commands_user[0]);

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
	{.key = "$USR", .next = commands_user, .next_size = commands_user_size, .description = "User menu"},
	{.key = "$LOG", .next = commands_log, .next_size = commands_log_size, .description = "Log menu"}

};
constexpr const size_t commands_size = sizeof(commands) / sizeof(commands[0]);

}

void set_app_parser(class app_parser& app_parser) OS_NOEXCEPT
{
	parser = const_cast<class parser*>(&app_parser.get_parser());
}


os::exit set_app_config(class app_config& app_config, error** error) OS_NOEXCEPT
{
	string<16> key;

	hhg::app::app_config = &app_config;

	key = "$VER";
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

	return exit::OK;
}

os::exit set_app_data(class app_data& app_data, error** error) OS_NOEXCEPT
{

	hhg::app::app_data = &app_data;

	return exit::OK;
}

void set_time(class time* time) OS_NOEXCEPT
{
	hhg::app::time = time;
}

entry* get_commands() OS_NOEXCEPT
{
	return commands;
}

size_t get_commands_size() OS_NOEXCEPT
{
	return commands_size;
}

}
}
