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

#include "hhg-iface/wifi.hpp"

namespace hhg::driver
{
inline namespace v1
{
    class pico_wifi final : public hhg::iface::wifi
    {
        constexpr static uint32_t TIMEOUT = 30'000;
    public:

        pico_wifi();
        ~pico_wifi() override OS_NOEXCEPT;
        OS_NO_COPY_NO_MOVE(pico_wifi)

        os::exit init(os::error **error) OS_NOEXCEPT override;

        os::exit connect(const os::string<128>& user, const os::string<64>& passwd, enum auth auth, os::error **error) const OS_NOEXCEPT override;
    };
}
}