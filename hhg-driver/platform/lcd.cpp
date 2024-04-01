/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY {} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "hhg-driver/lcd.hpp"

namespace hhg::driver
{
inline namespace v1
{

    lcd::lcd(const struct io& io, uint8_t cols, uint8_t rows) OS_NOEXCEPT
//    : io(io)
    {

    }

    void lcd::clear() OS_NOEXCEPT
    {

    }

    void lcd::home() OS_NOEXCEPT
    {

    }

    void lcd::noDisplay() OS_NOEXCEPT
    {

    }

    void lcd::display() OS_NOEXCEPT
    {

    }

    void lcd::blink() OS_NOEXCEPT
    {

    }

    void lcd::noBlink() OS_NOEXCEPT
    {

    }

    void lcd::noCursor() OS_NOEXCEPT
    {

    }

    void lcd::cursor() OS_NOEXCEPT
    {

    }

    void lcd::scrollDisplayLeft() OS_NOEXCEPT
    {

    }

    void lcd::scrollDisplayRight() OS_NOEXCEPT
    {

    }

    void lcd::printLeft() OS_NOEXCEPT
    {

    }

    void lcd::printRight() OS_NOEXCEPT
    {}

    void lcd::leftToRight() OS_NOEXCEPT
    {}

    void lcd::rightToLeft() OS_NOEXCEPT
    {}

    void lcd::shiftIncrement() OS_NOEXCEPT
    {}

    void lcd::shiftDecrement() OS_NOEXCEPT
    {}

    void lcd::noBacklight() OS_NOEXCEPT
    {}

    void lcd::backlight() OS_NOEXCEPT
    {}

    void lcd::autoscroll() OS_NOEXCEPT
    {}

    void lcd::noAutoscroll() OS_NOEXCEPT

    {}
    void lcd::createChar(uint8_t, uint8_t[]) OS_NOEXCEPT
    {}

    void lcd::setCursor(uint8_t, uint8_t) OS_NOEXCEPT
    {}

    void lcd::command(uint8_t) OS_NOEXCEPT
    {}

    void lcd::init() OS_NOEXCEPT
    {}

}
} // hhg