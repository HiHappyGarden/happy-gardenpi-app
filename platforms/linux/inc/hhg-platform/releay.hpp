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
#include "hhg-intf/releay.hpp"
#include "hhg-platform/types.hpp"
#include "osal/osal.hpp"

namespace hhg::platform
{
inline namespace v1
{

class releay final : public hhg::intf::releay
{

    const int32_t& fd;
    enum type type;

public:
    inline releay(const int32_t& fd) : fd(fd) {}

    bool init(os::error **)  OS_NOEXCEPT override;

    void set_status(uint8_t idx, bool status, class os::error** error) const OS_NOEXCEPT override;

    bool get_status(uint8_t idx, class os::error** error) const OS_NOEXCEPT override;

    uint8_t count_output() OS_NOEXCEPT override;
};

}
}

