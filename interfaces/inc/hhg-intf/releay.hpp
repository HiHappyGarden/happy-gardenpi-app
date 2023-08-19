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

#include "osal/osal.hpp"

#include <stdint.h>


namespace hhg::intf
{
inline namespace v1
{

struct releay
{
    using ptr = os::unique_ptr<releay>;

    virtual ~releay() OS_NOEXCEPT = default;

    virtual bool init(class os::error** error) OS_NOEXCEPT = 0;

    virtual void set_status(uint8_t idx, bool, class os::error** error) const OS_NOEXCEPT = 0;

    virtual bool get_status(uint8_t idx, class os::error** error) const OS_NOEXCEPT = 0;

    virtual uint8_t count_output() const OS_NOEXCEPT = 0;
};

}
}

