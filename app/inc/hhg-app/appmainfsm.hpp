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
    const intf::hardware& hardware;
    struct app_main::fsm& fsm;

    bool check_error_counter(error **error) OS_NOEXCEPT;
public:
    using ptr = unique_ptr<app_main_fsm>;

    inline app_main_fsm(struct app_main::fsm& fsm, intf::hardware& hardware) OS_NOEXCEPT : fsm(fsm), hardware(hardware) {}

    bool init(error **error) OS_NOEXCEPT;

    bool read_hw(error **error) OS_NOEXCEPT;
};

}
}

