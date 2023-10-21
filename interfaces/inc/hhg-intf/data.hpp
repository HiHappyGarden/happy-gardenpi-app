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

namespace hhg::intf
{
inline namespace v1
{

struct data
{
    using ptr = os::unique_ptr<data>;

    static constexpr const uint16_t FILE_SIZE = 0x1000;

    virtual ~data() OS_NOEXCEPT = default;

    virtual bool exist_data()  const OS_NOEXCEPT = 0;

    virtual bool exist_conf()  const OS_NOEXCEPT = 0;

    virtual bool read_data(os::string<FILE_SIZE>& data_json, os::error** error) const OS_NOEXCEPT = 0;

    virtual bool read_conf(os::string<FILE_SIZE>& data_conf, os::error** error) const OS_NOEXCEPT = 0;

    virtual bool write_data(const os::string<FILE_SIZE>& data_json, os::error** error) const OS_NOEXCEPT = 0;

    virtual bool write_conf(const os::string<FILE_SIZE>& data_conf, os::error** error) const OS_NOEXCEPT = 0;
};

}
}
