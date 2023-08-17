/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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
#include <stdint.h>

namespace hhg
{
inline namespace v1
{

enum class error_code : uint16_t
{
    INIT,
    NO_HEAP,
    NO_WRITE,
    NO_READ,
    HARDWARE_NO_DRIVER = 100,
    HARDWARE_REGISTRATION,
    HARDWARE_IOCL,
};


}
}

