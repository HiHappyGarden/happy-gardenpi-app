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
#include "hhg-intf/button.hpp"
#include "hhg-platform/types.hpp"
#include "osal/osal.hpp"

#include <stdint.h>
#include <signal.h>

namespace hhg::platform
{
inline namespace v1
{


class button final : public hhg::intf::button
{
    const int32_t& fd;
    enum type type;

    hhg::intf::button::on_click on_click{nullptr};

    friend void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT;
public:
    inline button(const int32_t& fd, enum type type) OS_NOEXCEPT : fd(fd), type(type)  {}
    OS_NO_COPY_NO_MOVE(button);

    bool init(class os::error** error) OS_NOEXCEPT override;

    void set_on_click(hhg::intf::button::on_click on_click) OS_NOEXCEPT override;

    bool get_status(os::error** error) const OS_NOEXCEPT override;
};

}
}

