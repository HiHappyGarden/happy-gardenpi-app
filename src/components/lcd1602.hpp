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

            uint16_t colls = 16;
            uint16_t rows = 2;

            int lcdContrast;
            int contrastTurnOn = true;

            /**
             * @brief Initialize display 16x2
             * @param lcdRS set GPIO pin number for lcd RS pin
             * @param lcdE set GPIO pin number for lcd E pin
             * @param lcd04 set GPIO pin number for lcd 04 pin
             * @param lcd05 set GPIO pin number for lcd 05 pin
             * @param lcd06 set GPIO pin number for lcd 06 pin
             * @param lcd07 set GPIO pin number for lcd 07 pin
             * @param lcdContrast set GPIO pin number for lcd Contrast pin
             * @exception runtime_error when hardware requisites mismatch
             */
            void init(int lcdRS, int lcdE, int lcd04, int lcd05, int lcd06, int lcd07, int lcdContrast);
        public:

            /**
             * @brief Constructor for the display 16x2
             * @param lcdRS set GPIO pin number for lcd RS pin
             * @param lcdE set GPIO pin number for lcd E pin
             * @param lcd04 set GPIO pin number for lcd 04 pin
             * @param lcd05 set GPIO pin number for lcd 05 pin
             * @param lcd06 set GPIO pin number for lcd 06 pin
             * @param lcd07 set GPIO pin number for lcd 07 pin
             * @param lcdContrast set GPIO pin number for lcd Contrast pin
             * @exception runtime_error when hardware requisites mismatch
             */
            inline LCD1602(int lcdRS, int lcdE, int lcd04, int lcd05, int lcd06, int lcd07, int lcdContrast) : handle(0), lcdContrast(lcdContrast)
            {
                init(lcdRS, lcdE, lcd04, lcd05, lcd06, lcd07, lcdContrast);
            }

            //remove copy constructor
            HGARDENPI_NO_COPY_NO_MOVE(LCD1602)

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
            void print(const string &txt) noexcept override;

            /**
             * Print a string on display
             * @param[in]     txt    Text to show
             */
            inline void print(const string &&txt) noexcept override
            {
                print(txt);
            }


            /**
             * Print a string on display formatted printf like
             * @param[in]     txt    Text to show formatted printf like
             * @param[in]     ...    Variatic arguments
             */
            void printf(const string &txt, ...) noexcept override;

            /**
             * Print add char on display
             * @param[in]     v    Char to add
             */
            void print(uint8_t c) noexcept override;

            /**
             * This home the cursor on display
             */
            void home() noexcept override;

            /**
             * This clear the cursor on display
             */
            void clear() noexcept override;

            /**
             * Set position on display
             *
             * @param[in]     x      Number rows
             * @param[in]     y      N umber colls
             */
            void position(int x, int y) noexcept override;

            /**
             * This turn the display on or off
             *
             * @param[in]    state   True turn on the display otherwise turn off
             */
            void display(bool state) noexcept override;

            /**
             * This turn the cursor on or off
             *
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            void cursor(bool state) noexcept override;

            /**
             * This turn the cursor on or off and blink it
             *
             * @param[in]    state   True turn on the cursor otherwise turn off
             */
            void cursorBlink(bool state) noexcept override;

            /**
             * Send command to lcd
             *
             * @param[in]    command   Send a byte command to lcd
             */
            void sendCommand(uint8_t command) noexcept override;

            /**
             * @brief This allows you to re-define one of the 8 user-definable chanracters in the display.
             * @details This allows you to re-define one of the 8 user-definable chanracters in the display. The data array is 8 bytes which represent the character from the top-line to the bottom line. Note that the characters are actually 5×8, so only the lower 5 bits are used. The index is from 0 to 7 and you can subsequently print the character defined using the lcdPutchar() call.
             *
             * @param[in]    index   Position to redifine
             * @param[in]    data    The data array is 8 bytes which represent the character
             */
            void charDef(int index, uint8_t data[8]) noexcept override;

            /**
             * @brief get if is turno on the display contrast
             * @return bool
             */
            [[nodiscard]] inline int isContrastTurnOn() const noexcept override
            {
                return contrastTurnOn;
            }

            /**
             * @brief set if is turn on the display contrast
             * @param contrastTurnOn true if you wont turn on the display contrast
             */
            void setContrastTurnOn(int contrastTurnOn) noexcept override;

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