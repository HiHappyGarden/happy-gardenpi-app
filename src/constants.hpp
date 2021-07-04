/*
MIT License

Copyright (c) 2021 Happy GardenPI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <cstdint>

//generic no copy no move constructor
#define HGARDENPI_NO_COPY_NO_MOVE(clazz)      \
    clazz(clazz &) = delete;                  \
    clazz &operator=(const clazz &) = delete; \
    clazz(clazz &&) = delete;                 \
    clazz &operator=(clazz &&) = delete;

#define HGARDENPI_ERROR_LOG_AMD_THROW(msg)                \
    LogService::getInstance()->write(LOG_ERR, "%d", msg); \
    throw runtime_error(msg);

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
            SECOND = 1'000,
            TICK = 200
        };

        //hw needed for Happy GardenPI
        const constexpr char *HW_V1 = "BCM2835";

    }
}