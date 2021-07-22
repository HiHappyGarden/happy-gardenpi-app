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

#include "display.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        /**
         * @brief LCD 16x02 implementation 4bit configuration
         */
        class LCD1602 final : public Display
        {

            int handle;

            uint16_t rows = 2;
            uint16_t colls = 16;

            void init(const vector<int> &);
        public:


            explicit LCD1602(const vector<int> &);
            explicit LCD1602(const vector<int> &&pins) : LCD1602(pins) {}

            //remove copy constructor
            HGARDENPI_NO_COPY_NO_MOVE(LCD1602)

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
            void print(const string &txt) noexcept;

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
            inline void print(const string &&txt) noexcept
            {
                print(txt);
            }


            /**
             * Print a string on display formatted printf like
             * @param[in]     txt    Text to show formatted printf like
             * @param[in]     ...    Variatic arguments
             */
            void printf(const string &txt, ...) noexcept;

            /**
             * Print add char on display
             * @param[in]     v    Char to add
             */
            void print(uint8_t c) noexcept;

            /**
             * This home the cursor on display
             */
            void home() noexcept;

            /**
             * This clear the cursor on display
             */
            void clear() noexcept;

            /**
             * Set position on display
             *
             * @param[in]     x      Number rows
             * @param[in]     y      N umber colls
             */
            void position(int x, int y) noexcept;

            /**
             * This turn the display on or off
             *
             * @param[in]    state   True turn on the display otherwise turn off
             */
            void display(bool state) noexcept;

            /**
             * This turn the cursor on or off
             *
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            void cursor(bool state) noexcept;

            /**
             * This turn the cursor on or off and blink it
             *
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            void cursorBlink(bool state) noexcept;

            /**
             * Send command to lcd
             *
             * @param[in]    command   Send a byte command to lcd
             */
            void sendCommand(uint8_t command) noexcept;

            /**
             * @brief This allows you to re-define one of the 8 user-definable chanracters in the display.
             * @details This allows you to re-define one of the 8 user-definable chanracters in the display. The data array is 8 bytes which represent the character from the top-line to the bottom line. Note that the characters are actually 5×8, so only the lower 5 bits are used. The index is from 0 to 7 and you can subsequently print the character defined using the lcdPutchar() call.
             *
             * @param[in]    index   Position to redifine
             * @param[in]    data    The data array is 8 bytes which represent the character
             */
            void charDef(int index, uint8_t data[8]) noexcept;

            /**
             * @brief Return the name of object
             *
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }
        };
    }
}