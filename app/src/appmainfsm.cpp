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


#include "hhg-app/appmainfsm.hpp"


namespace hhg::app
{
inline namespace v1
{

namespace
{
constexpr const char APP_TAG[] = "APP MAIN";
}

bool check_error_counter(os::error** error, uint8_t& count_error, enum app_main::state& state, enum app_main::state& old_state, uint8_t error_max)
{
    if(*error)
    {
        os::printf_stack_error(APP_TAG, *error);
        delete *error;
        *error = nullptr;
        if(count_error >= error_max)
        {
            old_state = state;
            state = app_main::RESET;
            count_error = 0;
            return false;
        }
        else
        {
            count_error++;
            return true;
        }
    }
    return true;
}


}
}

