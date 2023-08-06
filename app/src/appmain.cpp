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
#include <unistd.h>

namespace hhg::app
{
inline namespace v1
{

app_main::app_main() OS_NOEXCEPT
{

}

app_main::~app_main() OS_NOEXCEPT
{

}

bool app_main::init(os::error **) OS_NOEXCEPT
{

    return true;
}

bool app_main::fsm_start(osal::error **) OS_NOEXCEPT
{
    return true;
}

}
}

