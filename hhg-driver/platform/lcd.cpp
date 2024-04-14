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
using namespace os;

namespace hhg::driver
{
inline namespace v1
{

    lcd::lcd(const struct io& io, uint8_t cols, uint8_t rows) OS_NOEXCEPT
    : io(io)
    , cols(cols)
    , rows(rows)
    {

    }

    os::exit lcd::init(os::error **error) const OS_NOEXCEPT
    {
        if(io.init(error) == exit::KO)
        {
            return exit::KO;
        }

        back_light = LCD_ENABLE_BIT;

        send_byte(0x03, LCD_COMMAND);
        send_byte(0x03, LCD_COMMAND);
        send_byte(0x03, LCD_COMMAND);
        send_byte(0x02, LCD_COMMAND);

        send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
        send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
        send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
        clear();

        return exit::OK;
    }

    void lcd::clear() const OS_NOEXCEPT
    {
        send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
    }

    void lcd::home() const OS_NOEXCEPT
    {

    }

    void lcd::noDisplay() const OS_NOEXCEPT
    {

    }

    void lcd::display() const OS_NOEXCEPT
    {

    }

    void lcd::blink() const OS_NOEXCEPT
    {

    }

    void lcd::noBlink() const OS_NOEXCEPT
    {

    }

    void lcd::noCursor() const OS_NOEXCEPT
    {

    }

    void lcd::cursor() const OS_NOEXCEPT
    {

    }

    void lcd::scrollDisplayLeft() const OS_NOEXCEPT
    {

    }

    void lcd::scrollDisplayRight() const OS_NOEXCEPT
    {

    }

    void lcd::printLeft() const OS_NOEXCEPT
    {

    }

    void lcd::printRight() const OS_NOEXCEPT
    {}

    void lcd::leftToRight() const OS_NOEXCEPT
    {}

    void lcd::rightToLeft() const OS_NOEXCEPT
    {}

    void lcd::shiftIncrement() const OS_NOEXCEPT
    {}

    void lcd::shiftDecrement() const OS_NOEXCEPT
    {}

    void lcd::noBacklight() const OS_NOEXCEPT
    {}

    void lcd::backlight() const OS_NOEXCEPT
    {}

    void lcd::autoscroll() const OS_NOEXCEPT
    {}

    void lcd::noAutoscroll() const OS_NOEXCEPT

    {}
    void lcd::createChar(uint8_t, uint8_t[]) const OS_NOEXCEPT
    {}

    void lcd::setCursor(uint8_t, uint8_t) const OS_NOEXCEPT
    {}

    void lcd::command(uint8_t) const OS_NOEXCEPT
    {}

    void lcd::send_byte(uint8_t byte, uint8_t mode) const OS_NOEXCEPT
    {
        uint8_t high = mode | (byte & 0xf0) | back_light;
        uint8_t low = mode | ((byte << 4) & 0xf0) | back_light;

        write_byte(high | mode);
        toggle_enable(high);

        write_byte(low | mode);
        toggle_enable(low);
    }

    void lcd::toggle_enable(uint8_t val) const OS_NOEXCEPT
    {
        io.us_sleep(600);
        write_byte(val | back_light);
        io.us_sleep(600);
        write_byte(val & ~back_light);
        io.us_sleep(600);
    }

    inline void lcd::write_byte(uint8_t val) const OS_NOEXCEPT
    {
        io.write(&val, 1);
    }


}
} // hhg