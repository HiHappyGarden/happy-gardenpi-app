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


#pragma once

#include "hhg-parser/parser.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_config;

using namespace hhg::parser;

void set_app_parser(class app_parser& app_parser) OS_NOEXCEPT;

os::exit set_app_config(app_config& app_config, os::error** error = nullptr) OS_NOEXCEPT;

entry* get_commands() OS_NOEXCEPT;
size_t get_commands_size() OS_NOEXCEPT;

}
}
