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

#include "hhg-app/appmain.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_main_fsm
{
    struct app_main::fsm& fsm;
public:

    explicit app_main_fsm(struct app_main::fsm& fsm);
};


bool __attribute__((nonnull (1))) check_error_counter(os::error** error, uint8_t& count_error, enum app_main::state& state, enum app_main::state& old_state, uint8_t error_max);


}
}

