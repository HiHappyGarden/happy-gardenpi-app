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

namespace hhg::app
{
inline namespace v1
{

entry commands_user[] =
{
		{.key = "1", .custom_func = [](auto data, auto entry, auto error) -> os::exit {

			strncpy(data.ret_buffer, "bye", data.ret_buffer_len);

			return os::exit::OK;
		} , .description = "Custom func"}
};
constexpr const size_t commands_user_size = sizeof(commands_user) / sizeof(commands_user[0]);

entry commands[] =
{

		{.key = "^VER", .description = "Get App version"},
		{.key = "^USR", .next = commands_user, .next_size = commands_user_size}

};
constexpr const size_t commands_size = sizeof(commands) / sizeof(commands[0]);


entry* get_commands()
{
	return commands;
}

size_t get_commands_size()
{
	return commands_size;
}

}
}
