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
#include "config.h"
#include "osal/osal.hpp"

namespace hhg::intf
{
inline namespace v1
{
class hardware;
}
}


namespace hhg::app
{
inline namespace v1
{

using namespace os;

class app_main final
{
    const hhg::intf::hardware& hardware;

    os::string<HHGARDEN_LCD_MSG_SIZE> lcd_msg;

public:
    explicit app_main(const hhg::intf::hardware& hardware) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() OS_NOEXCEPT;

    bool init(class error** error) OS_NOEXCEPT;

    bool fsm_start(class error** error) OS_NOEXCEPT;
};

}
}

