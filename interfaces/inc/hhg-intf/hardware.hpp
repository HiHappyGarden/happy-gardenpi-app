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
#include "hhg-intf/lcd.hpp"
#include "hhg-intf/led.hpp"
#include "hhg-intf/releay.hpp"
#include "hhg-intf/data.hpp"
#include <stdint.h>

namespace hhg::intf
{
inline namespace v1
{

class hardware
{
protected:
    virtual ~hardware() OS_NOEXCEPT = default;

public:
    virtual bool init(os::error** error) OS_NOEXCEPT = 0;

    virtual const os::string<128>& get_info() OS_NOEXCEPT = 0;

    virtual const os::string<128>& get_version() OS_NOEXCEPT = 0;

    virtual int32_t get_temperature(os::error** error) OS_NOEXCEPT = 0;

    virtual const button::ptr& get_button_next() const OS_NOEXCEPT = 0;

    virtual const button::ptr& get_button_before() const OS_NOEXCEPT = 0;

    virtual const lcd::ptr& getLcd() const OS_NOEXCEPT = 0;

    virtual const led::ptr& get_led_green() const OS_NOEXCEPT = 0;

    virtual const led::ptr& get_led_red() const OS_NOEXCEPT = 0;

    virtual const releay::ptr& get_releay() const OS_NOEXCEPT = 0;

    virtual const data::ptr& get_data() const OS_NOEXCEPT = 0;
};

}
}

