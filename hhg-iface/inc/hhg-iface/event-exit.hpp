/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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

#include "osal/osal.hpp"

namespace hhg::iface
{
inline namespace v1
{

struct event_exit
{
    using on_exit_callback = void(event_exit::*)(os::exit, const char*, void *);

    virtual ~event_exit() = default;

    virtual void on_exit(os::exit, const char*, void *) OSAL_NOEXCEPT = 0;
};

}
}