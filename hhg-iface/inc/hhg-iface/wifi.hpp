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

#include "hhg-iface/initializable.hpp"

namespace hhg::iface
{
inline namespace v1
{

struct wifi : public initializable
{

    enum class auth : uint32_t
    {
        OPEN = 1,
        WPA_TKIP_PSK,
        WPA2_AES_PSK,
        WPA2_MIXED_PSK
    };

    using ptr = os::unique_ptr<wifi>;

    ~wifi() override = default;

    virtual os::exit connect(const os::string<128>& user, const os::string<64>& passwd, enum auth auth, os::error **error) const OS_NOEXCEPT = 0;

    //virtual os::exit get_ntp() const OS_NOEXCEPT = 0;



};




}
}
