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

#include "hhg-platform/lcd.hpp"
#include "osal/osal.hpp"

#include <sys/ioctl.h>

namespace hhg::platform
{
inline namespace v1
{

namespace
{
constexpr const char APP_TAG[] = "BUTTON";
}


bool lcd::init(os::error** error) OS_NOEXCEPT
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

void lcd::set_text(const string<34>& str, os::error** error) const OS_NOEXCEPT
{

}

void lcd::set_text(const char (&buff) [34], os::error** error) const OS_NOEXCEPT
{

}

string<34> lcd::get_text(os::error** error) const OS_NOEXCEPT
{
    return {};
}

}
}

