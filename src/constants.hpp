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

#include <cstdint>
#include <clocale>
#include <libintl.h>

//generic no copy no move constructor
#define HGARDENPI_NO_COPY_NO_MOVE(clazz)      \
    clazz(clazz &) = delete;                  \
    clazz &operator=(const clazz &) = delete; \
    clazz(clazz &&) = delete;                 \
    clazz &operator=(clazz &&) = delete;

#define HGARDENPI_ERROR_LOG_AMD_THROW(msg) \
    logService->write(LOG_ERR, "%d", msg); \
    throw runtime_error(_(msg));

#define _(str) gettext(str)

namespace hgardenpi
{
    inline namespace v1
    {
        /**
         * @brief Enum for manage times
         * 
         */
        enum class Time : int64_t
        {

            DISPLAY_TICK = 6'000,
            DISPLAY_SHORT_TICK = DISPLAY_TICK / 3,
            DISPLAY_CONTRAST_TICK = DISPLAY_TICK * 3,
            TICK = 50,
            SCHEDULER_TICK = 200,
            SCHEDULER_NETX_TICK = 2'000,
        };

        /**
         * @brief Day of week representation
         */
        enum DayOfWeek : uint8_t
        {
            MONDAY    = 0b00000001,
            TUESDAY   = 0b00000010,
            WEDNESDAY = 0b00000100,
            THURSDAY  = 0b00001000,
            FRIDAY    = 0b00010000,
            SATURDAY  = 0b00100000,
            SUNDAY    = 0b01000000
        };

        /**
        * @brief Status of an element in the project
        */
        enum class Status : uint8_t
        {
            /**
             * @brief element not active
             */
            UNACTIVE = 0,
            /**
             * @brief element active
             */
            ACTIVE,
            /**
             * @brief element error
             */
            ERROR,
        };

        //hw tested for Happy GardenPI
        /**
         * @brief raspberry pi 4 hw
         */
        const constexpr inline char *HW_V1_DEBUG = "BCM2711";

        /**
         * @brief raspberry pi zero hw
         */
        const constexpr inline char *HW_V1 = "BCM2835";

        /**
         * @brief une second
         */
        const constexpr inline uint16_t ONE_SECOND = 1'000;

        /**
         * @brief number of thread for pool
         */
        const constexpr inline uint8_t NUMBER_OF_THREAD = 6;

    }
}