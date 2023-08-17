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

struct  lcd
{
    using ptr = os::unique_ptr<lcd>;

    virtual ~lcd() OS_NOEXCEPT = default;

    virtual bool init(os::error** error) OS_NOEXCEPT = 0;

    virtual void set_text(const os::string<HHGARDEN_LCD_MSG_SIZE>& string, os::error** error) const OS_NOEXCEPT = 0;

    virtual void set_text(const char (&buff) [HHGARDEN_LCD_MSG_SIZE], os::error** error) const OS_NOEXCEPT = 0;

    virtual os::string<HHGARDEN_LCD_MSG_SIZE> get_text(os::error** error) const OS_NOEXCEPT = 0;
};

}
}

