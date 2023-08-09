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

#include <stdint.h>>

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

class releay
{
public:
    static uint8_t count_output() OS_NOEXCEPT;

    static const releay& get_output(uint8_t) OS_NOEXCEPT;

protected:
    virtual ~releay() OS_NOEXCEPT = default;

    virtual bool init(class osal::error**) OS_NOEXCEPT = 0;

    virtual void set_status(bool) OS_NOEXCEPT = 0;

    virtual bool get_status() const OS_NOEXCEPT = 0;

};

}
}

