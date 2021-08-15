// MIT License
//
// Copyright (c) 2021 Happy GardenPI
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "lcd1602.hpp"
using std::lock_guard;

#include <stdexcept>
using std::runtime_error;

#include <cstdarg>

#include <wiringPi.h>
#include <lcd.h>

using hgardenpi::v1::LCD1602;

LCD1602::LCD1602(int lcdRS, int lcdE, int lcd04, int lcd05, int lcd06, int lcd07, int lcdContrast) : handle(-1), lcdContrast(lcdContrast)
{

    handle = lcdInit(rows,
                     colls,
                     4, //bits
                     lcdRS, //LCD_RS
                     lcdE, //LCD_E
                     lcd04, //LCD_04
                     lcd05, //LCD_05
                     lcd06, //LCD_06
                     lcd07, //LCD_07
                     0, 0, 0, 0);

    pinMode(lcdContrast, OUTPUT);

    print("");

    clear();

    if (handle == -1)
    {
        throw runtime_error("Init error");
    }
}

LCD1602::~LCD1602() noexcept
{
    clear();
    setContrastTurnOn(false);
}


inline void LCD1602::print(const string &txt) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdPuts(handle, txt.c_str());
}

inline void LCD1602::printf(const string &txt, ...) noexcept
{
    lock_guard<mutex> lg(m);
    va_list(args);
    va_start(args, txt);
    ::lcdPrintf(handle, txt.c_str(), args);
}

inline void LCD1602::print(const uint8_t c) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdPutchar(handle, c);
}

inline void LCD1602::home() noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdHome(handle);
}

inline void LCD1602::clear() noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdClear(handle);
}

inline void LCD1602::position(int x, int y) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdPosition(handle, x, y);
}

inline void LCD1602::display(bool state) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdDisplay(handle, state);
}

inline void LCD1602::cursor(bool state) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdCursor(handle, state);
}

inline void LCD1602::cursorBlink(bool state) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdCursorBlink(handle, state);
}

inline void LCD1602::sendCommand(uint8_t command) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdCursorBlink(handle, command);
}
inline void LCD1602::charDef(int index, uint8_t data[8]) noexcept
{
    lock_guard<mutex> lg(m);
    ::lcdCharDef(handle, index, data);
}

inline void LCD1602::setContrastTurnOn(bool contrastTurnOn) noexcept
{
    lock_guard<mutex> lg(m);
    ::digitalWrite(lcdContrast, contrastTurnOn);
    this->contrastTurnOn = contrastTurnOn;
}