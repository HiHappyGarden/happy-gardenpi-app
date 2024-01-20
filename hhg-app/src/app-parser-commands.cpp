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
#include "hhg-app/app-config.hpp"
using namespace osal;

namespace hhg::app
{
inline namespace v1
{

namespace
{

class app_config* _app_config = nullptr;

entry commands_config[] =
{
	//{.key = "1", .func  = new method(_app_config, get_serial),  .description = "Get serial"},
};
constexpr const size_t commands_config_size = sizeof(commands_config) / sizeof(commands_config[0]);

entry commands_user[] =
{
		{.key = "1", .func  = new method(_app_config, &app_config::get_serial), .description = "Get serial"},
		{.key = "2", .func  = new method(_app_config, &app_config::set_serial), .description = "Set serial"},
		{.key = "3", .func  = new method(_app_config, &app_config::get_descr), .description = "Get description"},
		{.key = "4", .func  = new method(_app_config, &app_config::set_descr), .description = "Set description"},
};
constexpr const size_t commands_user_size = sizeof(commands_user) / sizeof(commands_user[0]);

entry commands[] =
{

		{.key = "^VER", .func  = new method(_app_config, &app_config::get_version), .description = "Get version"},
		{.key = "^CONF", .next = commands_config, .next_size = commands_config_size, .description = "Configuration menu"},
		{.key = "^USR", .next = commands_user, .next_size = commands_user_size, .description = "User menu"}

};
constexpr const size_t commands_size = sizeof(commands) / sizeof(commands[0]);

}

void set_app_config(class app_config& app_config) OS_NOEXCEPT
{
	hhg::app::_app_config = &app_config;
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
