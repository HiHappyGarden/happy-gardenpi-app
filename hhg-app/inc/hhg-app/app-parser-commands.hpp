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

namespace hhg::iface
{
inline namespace v1
{
    class time;
}
}

namespace hhg::driver
{
inline namespace v1
{
    class hardware;
}
}

namespace hhg::app
{
inline namespace v1
{

    class app_config;
    class app_display_handler;

    using namespace hhg::parser;

    os::exit set_app_parser(class app_parser& app_parser, os::error** error) OSAL_NOEXCEPT;

    os::exit set_app_config(class app_config& app_config, os::error** error = nullptr) OSAL_NOEXCEPT;

    os::exit set_app_data(class app_data& app_data, os::error** error) OSAL_NOEXCEPT;

    void set_app_display_handler(class app_display_handler& app_display_handler) OSAL_NOEXCEPT;

    void set_time(class hhg::iface::time* time) OSAL_NOEXCEPT;

    os::exit auth(const cmd_data &data, const entry *entry, os::error **error) OSAL_NOEXCEPT;

    entry* get_commands() OSAL_NOEXCEPT;

    size_t get_commands_size() OSAL_NOEXCEPT;

}
}
