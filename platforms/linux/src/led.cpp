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

#include "hhg-platform/led.hpp"
#include <sys/ioctl.h>

namespace hhg::platform
{
inline namespace v1
{

namespace
{
constexpr const char APP_TAG[] = "LED";
}


bool led::init(class os::error** error) OS_NOEXCEPT
{
    if(fd == -1)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Fd not init", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }
    return true;
}

void led::set_status(bool status, os::error** error) const OS_NOEXCEPT
{
    if(ioctl(fd, static_cast<uint8_t>(type) | static_cast<uint8_t>(action::WRITE), &status) < 0)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("ioctl() fail", static_cast<uint8_t>(error_code::HARDWARE_IOCL), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }
}

bool led::get_status(os::error** error) const OS_NOEXCEPT
{
    uint32_t status = 0;
    if(ioctl(fd, static_cast<uint8_t>(type) | static_cast<uint8_t>(action::READ), &status) < 0)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("ioctl() fail", static_cast<uint8_t>(error_code::HARDWARE_IOCL), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    return status;
}

}
}

