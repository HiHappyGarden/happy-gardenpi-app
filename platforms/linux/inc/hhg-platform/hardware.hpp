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


#include <signal.h>

namespace hhg::platform
{
inline namespace v1
{

using os::error;
using os::string;
namespace intf = hhg::intf;

class hardware : public hhg::intf::hardware
{
    int32_t fd{-1};

    os::unique_ptr<intf::button> button_next{nullptr};
    os::unique_ptr<intf::button> button_before{nullptr};
    os::unique_ptr<intf::lcd> lcd{nullptr};
    os::unique_ptr<intf::led> led_green{nullptr};
    os::unique_ptr<intf::led> led_red{nullptr};
    os::unique_ptr<intf::releay> releay{nullptr};

    friend void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT;
public:
    hardware() OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(hardware);

    ~hardware() OS_NOEXCEPT;

    bool init(class error** error) OS_NOEXCEPT override;

    const os::string<128>& get_info() OS_NOEXCEPT override;

    const os::string<128>& get_version() OS_NOEXCEPT override;

    int32_t get_temperature(error **error) OS_NOEXCEPT override;

    inline const os::unique_ptr<intf::button>& get_button_next() const OS_NOEXCEPT override
    {
        return button_next;
    }

    inline const os::unique_ptr<intf::button>& get_button_before() const OS_NOEXCEPT override
    {
        return button_before;
    }

    inline const os::unique_ptr<intf::lcd>& getLcd() const OS_NOEXCEPT override
    {
        return lcd;
    }

    inline const os::unique_ptr<intf::led>& get_led_green() const OS_NOEXCEPT override
    {
        return led_green;
    }

    inline const os::unique_ptr<intf::led>& get_led_red() const OS_NOEXCEPT override
    {
        return led_red;
    }

    inline const os::unique_ptr<intf::releay>& get_releay() const OS_NOEXCEPT override
    {
        return releay;
    }
};

}
}
