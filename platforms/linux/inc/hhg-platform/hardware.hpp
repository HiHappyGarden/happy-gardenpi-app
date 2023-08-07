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

#pragma once
#include "hhg-intf/hardware.hpp"
#include "osal/osal.hpp"

#include <signal.h>

namespace hhg::platform
{
inline namespace v1
{

class button;

using os::error;

class hardware : public hhg::intf::hardware
{
    int32_t fd{-1};
    button* button_next{nullptr};
    button* button_before{nullptr};

    friend void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT;
public:
    hardware() OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(hardware);

    ~hardware() OS_NOEXCEPT;

    bool init(class error** error) OS_NOEXCEPT override;

};

}
}
