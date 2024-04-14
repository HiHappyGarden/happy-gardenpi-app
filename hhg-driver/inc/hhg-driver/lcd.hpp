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
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/


#pragma once

#include "hhg-iface/initializable.hpp"

namespace hhg::driver
{
inline namespace v1
{

class lcd
{

public:


// commands
    static constexpr uint8_t LCD_CLEARDISPLAY = 0x01;
    static constexpr uint8_t LCD_RETURNHOME = 0x02;
    static constexpr uint8_t LCD_ENTRYMODESET = 0x04;
    static constexpr uint8_t LCD_DISPLAYCONTROL = 0x08;
    static constexpr uint8_t LCD_CURSORSHIFT = 0x10;
    static constexpr uint8_t LCD_FUNCTIONSET = 0x20;
    static constexpr uint8_t LCD_SETCGRAMADDR = 0x40;
    static constexpr uint8_t LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
    static constexpr uint8_t LCD_ENTRYSHIFTINCREMENT = 0x01;
    static constexpr uint8_t LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
    static constexpr uint8_t LCD_BLINKON = 0x01;
    static constexpr uint8_t LCD_CURSORON = 0x02;
    static constexpr uint8_t LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
    static constexpr uint8_t LCD_MOVERIGHT = 0x04;
    static constexpr uint8_t LCD_DISPLAYMOVE = 0x08;

// flags for function set
    static constexpr uint8_t LCD_5x10DOTS = 0x04;
    static constexpr uint8_t LCD_2LINE = 0x08;
    static constexpr uint8_t LCD_8BITMODE = 0x10;

// flag for backlight control
    static constexpr uint8_t LCD_BACKLIGHT = 0x08;

    static constexpr uint8_t LCD_ENABLE_BIT = 0x04;


// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

    using ptr = os::unique_ptr<hhg::driver::lcd>;

    struct io
    {
        io() = default;
        virtual ~io() = default;
        OS_NO_COPY_NO_MOVE(io)
        virtual os::exit init(os::error** error) const OS_NOEXCEPT = 0;
        virtual int32_t write(const uint8_t*, size_t data_len) const OS_NOEXCEPT = 0;
        virtual int32_t read(uint8_t*, size_t data_size) const OS_NOEXCEPT = 0;
        virtual void us_sleep(uint64_t us) const OS_NOEXCEPT = 0;
    };

    mutable uint8_t back_light = 0;

    virtual ~lcd() = default;
    OS_NO_COPY_NO_MOVE(lcd);

    os::exit init(os::error** error) const OS_NOEXCEPT;
    void clear() const OS_NOEXCEPT;
    void home() const OS_NOEXCEPT;
    void noDisplay() const OS_NOEXCEPT;
    void display() const OS_NOEXCEPT;
    void noBlink() const OS_NOEXCEPT;
    void blink() const OS_NOEXCEPT;
    void noCursor() const OS_NOEXCEPT;
    void cursor() const OS_NOEXCEPT;
    void scrollDisplayLeft() const OS_NOEXCEPT;
    void scrollDisplayRight() const OS_NOEXCEPT;
    void printLeft() const OS_NOEXCEPT;
    void printRight() const OS_NOEXCEPT;
    void leftToRight() const OS_NOEXCEPT;
    void rightToLeft() const OS_NOEXCEPT;
    void shiftIncrement() const OS_NOEXCEPT;
    void shiftDecrement() const OS_NOEXCEPT;
    void noBacklight() const OS_NOEXCEPT;
    void backlight() const OS_NOEXCEPT;
    void autoscroll() const OS_NOEXCEPT;
    void noAutoscroll() const OS_NOEXCEPT;
    void createChar(uint8_t, uint8_t[]) const OS_NOEXCEPT;
    void setCursor(uint8_t, uint8_t) const OS_NOEXCEPT;
    void command(uint8_t) const OS_NOEXCEPT;

protected:
    uint8_t cols = 0;
    uint8_t rows = 0;

    lcd(const struct io& io, uint8_t cols, uint8_t rows) OS_NOEXCEPT;


    void send_byte(uint8_t byte, uint8_t mode) const OS_NOEXCEPT;
    void toggle_enable(uint8_t val) const OS_NOEXCEPT;
    void write_byte(uint8_t val) const OS_NOEXCEPT;

private:
    const struct io& io;


};

} // hhg
}