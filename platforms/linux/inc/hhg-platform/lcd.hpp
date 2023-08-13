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
#include "hhg-intf/lcd.hpp"
#include "hhg-platform/types.hpp"

#include <stdint.h>
#include <signal.h>

namespace hhg::platform
{
inline namespace v1
{

using os::string;

class lcd final : public hhg::intf::lcd
{
    const int32_t& fd;
    enum type const type = type::LCD;

    friend void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT;
public:
    inline lcd(const int32_t& fd) : fd(fd) OS_NOEXCEPT {}
    OS_NO_COPY_NO_MOVE(lcd);

    bool init(class os::error** error) OS_NOEXCEPT override;

    void set_text(const string<HHGARDEN_LCD_MSG_SIZE>& str, os::error** error) const OS_NOEXCEPT override;

    void set_text(const char (&buff) [HHGARDEN_LCD_MSG_SIZE], os::error** error) const OS_NOEXCEPT override;

    string<HHGARDEN_LCD_MSG_SIZE> get_text(os::error** error) const OS_NOEXCEPT override;
};

}
}

