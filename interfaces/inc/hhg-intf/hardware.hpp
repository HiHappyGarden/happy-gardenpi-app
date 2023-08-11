/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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


namespace osal
{
inline namespace v1
{
class error;
}
}

namespace hhg::intf
{
inline namespace v1
{

class hardware
{
protected:
    virtual ~hardware() OS_NOEXCEPT = default;

public:
    virtual bool init(class osal::error**) OS_NOEXCEPT = 0;

    virtual const char* get_info() OS_NOEXCEPT = 0;

    virtual const char* get_version() OS_NOEXCEPT = 0;

    virtual int16_t get_temperature(class osal::error**) OS_NOEXCEPT = 0;
};

}
}

