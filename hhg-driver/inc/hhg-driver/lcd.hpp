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
#include "osal/osal.hpp"

#include "hhg-iface/fs-io.hpp"

namespace hhg::driver
{
inline namespace v1
{

class lcd final
{
    // commands
    enum commands
    {
     LCD_CLEAR_DISPLAY = 0x01,
     LCD_RETURN_HOME = 0x02,
     LCD_ENTRY_MODE_SET = 0x04,
     LCD_DISPLAY_CONTROL = 0x08,
     LCD_CURSOR_SHIFT  = 0x10,
     LCD_FUNCTION_SET  =0x20,
     LCD_SET_CG_RAM_ADDR = 0x40,
     LCD_SET_DD_RAM_ADDR = 0x80
    };

    // flags for display entry mode
    enum flags_entry_mode
    {
        LCD_ENTRY_RIGHT = 0x00,
        LCD_ENTRY_LEFT = 0x02,
        LCD_ENTRY_SHIFT_INCREMENT = 0x01,
        LCD_ENTRY_SHIFT_DECREMENT = 0x00
    };

    // flags for display on/off control
    enum flags_control
    {
        LCD_DISPLAY_ON = 0x04,
        LCD_DISPLAY_OFF = 0x00,
        LCD_CURSOR_ON = 0x02,
        LCD_CURSOR_OFF = 0x00,
        LCD_BLINK_ON = 0x01,
        LCD_BLINK_OFF = 0x00,
    };

    // flags for display/cursor shift
    enum flags_cursor_shift
    {
        LCD_DISPLAY_MOVE = 0x08,
        LCD_CURSOR_MOVE = 0x00,
        LCD_MOVE_RIGHT = 0x04,
        LCD_MOVE_LEFT = 0x00
    };

    // flags for function set
    enum flags_function_set
    {
        LCD_8BIT_MODE = 0x10,
        LCD_4BIT_MODE = 0x00,
        LCD_2LINE = 0x08,
        LCD_1LINE = 0x00,
        LCD_5x10DOTS = 0x04,
        LCD_5x8DOTS = 0x00
    };

    // flags for backlight control
    enum flags_backlight_control
    {
        LCD_BACKLIGHT = 0x08,
        LCD_NO_BACKLIGHT = 0x00
    };

    static constexpr uint8_t EN = 0b00000100;  // Enable bit
    static constexpr uint8_t RW = 0b00000010;  // Read/Write bit
    static constexpr uint8_t RS = 0b00000001;  // Register select bit

//    const struct io& io;
    uint8_t cols = 0;
    uint8_t rows = 0;

public:

    struct io
    {
        virtual os::exit write(const char data[], size_t data_len) const = 0;
        virtual os::exit read(char data[], size_t data_size) const = 0;
    };

    lcd(const struct io& io, uint8_t cols, uint8_t rows) OS_NOEXCEPT;
    void clear() OS_NOEXCEPT;
    void home() OS_NOEXCEPT;
    void noDisplay() OS_NOEXCEPT;
    void display() OS_NOEXCEPT;
    void noBlink() OS_NOEXCEPT;
    void blink() OS_NOEXCEPT;
    void noCursor() OS_NOEXCEPT;
    void cursor() OS_NOEXCEPT;
    void scrollDisplayLeft() OS_NOEXCEPT;
    void scrollDisplayRight() OS_NOEXCEPT;
    void printLeft() OS_NOEXCEPT;
    void printRight() OS_NOEXCEPT;
    void leftToRight() OS_NOEXCEPT;
    void rightToLeft() OS_NOEXCEPT;
    void shiftIncrement() OS_NOEXCEPT;
    void shiftDecrement() OS_NOEXCEPT;
    void noBacklight() OS_NOEXCEPT;
    void backlight() OS_NOEXCEPT;
    void autoscroll() OS_NOEXCEPT;
    void noAutoscroll() OS_NOEXCEPT;
    void createChar(uint8_t, uint8_t[]) OS_NOEXCEPT;
    void setCursor(uint8_t, uint8_t) OS_NOEXCEPT;
    void command(uint8_t) OS_NOEXCEPT;
    void init() OS_NOEXCEPT;
};

} // hhg
}