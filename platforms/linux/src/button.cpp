/***************************************************************************
 *
 * PROJECT
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-platform/button.hpp"
#include "hhg-platform/types.hpp"
#include "osal/osal.hpp"

namespace hhg::platform
{
inline namespace v1
{

bool button::init(osal::error** error) OS_NOEXCEPT
{
    if(fd == -1)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(" Fd not init", static_cast<uint8_t>(error_code::HHGD_INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }
    return true;
}

void button::set_on_click(hhg::intf::button::on_click on_click) OS_NOEXCEPT
{
    this->on_click = on_click;
}

}
}

