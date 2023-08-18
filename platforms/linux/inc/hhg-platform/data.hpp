/***************************************************************************
 *
 * Hi Happy Garden Interfaces
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


#include "hhg-intf/data.hpp"

namespace hhg::platform
{
inline namespace v1
{

using namespace os;

class data final : public hhg::intf::data
{
public:
    data() = default;
    OS_NO_COPY_NO_MOVE(data);
    bool read_data(string<FILE_SIZE>& data_json, os::error** error) const OS_NOEXCEPT override;

    bool read_conf(string<FILE_SIZE>& data_conf, os::error** error) const OS_NOEXCEPT override;

    bool write_data(const string<FILE_SIZE>& data_json, error** error) const OS_NOEXCEPT override;

    bool write_conf(const string<FILE_SIZE>& data_conf, error** error) const OS_NOEXCEPT override;
};


}
}
