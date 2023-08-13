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

#include <unistd.h>

namespace hhg::app
{
inline namespace v1
{

app_main::app_main(const hhg::intf::hardware& hardware) OS_NOEXCEPT : hardware(hardware)
{

}

app_main::~app_main() OS_NOEXCEPT
{

}

bool app_main::init(class error** error) OS_NOEXCEPT
{
    //hardware.getLcd()->set_text("ciao", error);

    return true;
}

bool app_main::fsm_start(class error** error) OS_NOEXCEPT
{
    return true;
}

}
}

