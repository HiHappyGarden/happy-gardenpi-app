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

#include "hhg-app/appmain.hpp"
#include "hhg-intf/hardware.hpp"
#include "hhg-intf/hardware.hpp"
#include "osal/osal.hpp"
#include "errors.hpp"

#include <unistd.h>

namespace hhg::app
{
inline namespace v1
{

app_main::app_main(const intf::hardware& hardware) OS_NOEXCEPT
    : hardware(hardware)
    , app_data(hardware)
{

}

app_main::~app_main() OS_NOEXCEPT
{

}

bool app_main::init(os::error** error) OS_NOEXCEPT
{

    lcd_msg.clear();
    if(hardware.getLcd()->set_text(lcd_msg, error); *error != nullptr)
    {
        *error = OS_ERROR_BUILD(*error, "Lcd set_text() fail", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}

bool app_main::fsm_start(os::error** error) OS_NOEXCEPT
{
    return true;
}

}
}

