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

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../interfaces/object.hpp"

namespace hgardenpi
{

    inline namespace v1
    {
        using std::string;
        using std::vector;

        /**
         * @brief Simple class for diplay managing
         * 
         * @details this class is generic for create instance you must use Display::factory 
         */
        class Display : public Object
        {
        public:
            Display() = default;
            HGARDENPI_NO_COPY_NO_MOVE(Display)

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
             virtual void print(const string &&str) noexcept = 0;

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
            virtual void print(const string &txt) noexcept = 0;

            /**
             * Print a string on display formatted printf like
             * @param[in]     txt    Text to show formatted printf like
             * @param[in]     ...    Variatic arguments
             */
            virtual void printf(const string &txt, ...) noexcept = 0;

            /**
             * Print add char on display
             * @param[in]     v    Char to add
             */
            virtual void print(uint8_t c) noexcept = 0;

            /**
             * This home the cursor on display
             */
            virtual void home() noexcept = 0;

            /**
             * This clear the cursor on display
             */
            virtual void clear() noexcept = 0;

            /**
             * Set position on display
             *
             * @param[in]     x      Number rows
             * @param[in]     y      N umber colls
             */
            virtual void position(int x, int y) noexcept = 0;

            /**
             * This turn the display on or off
             * 
             * @param[in]    state   True turn on the display otherwise turn off
             */
            virtual void display(bool state) noexcept = 0;

            /**
             * This turn the cursor on or off
             * 
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            virtual void cursor(bool state) noexcept = 0;

            /**
             * This turn the cursor on or off and blink it
             * 
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            virtual void cursorBlink(bool state) noexcept = 0;

            /**
             * Send command to lcd
             * 
             * @param[in]    command   Send a byte command to lcd
             */
            virtual void sendCommand(uint8_t command) noexcept = 0;

            /**
             * @brief This allows you to re-define one of the 8 user-definable chanracters in the display.
             * @details This allows you to re-define one of the 8 user-definable chanracters in the display. The data array is 8 bytes which represent the character from the top-line to the bottom line. Note that the characters are actually 5×8, so only the lower 5 bits are used. The index is from 0 to 7 and you can subsequently print the character defined using the lcdPutchar() call.
             * 
             * @param[in]    index   Position to redifine
             * @param[in]    data    The data array is 8 bytes which represent the character
             */
            virtual void charDef(int index, uint8_t data[8]) noexcept = 0;

        };
    }
}