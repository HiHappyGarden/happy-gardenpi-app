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

#include "logservice.hpp"
using namespace std;

#include <cstdarg>
#include <cstring>

#include "../config.h"

using namespace hgardenpi;

LogService::LogService() noexcept
{
    openlog(HGARDENPI_NAME, LOG_PID, LOG_USER);
    syslog(LOG_INFO, "start");
}

void LogService::write(uint8_t level, const char *msg, ...) const noexcept
{
    static const constexpr int MAX_STRING_LEN = 1024;
    lock_guard<mutex> lg(m);

    va_list ap;
    va_start(ap, msg);

    char *buffer = new (nothrow) char[MAX_STRING_LEN];
    if (buffer)
    {
        memset(&buffer[0], 0, sizeof(buffer));
        vsnprintf(buffer, MAX_STRING_LEN, msg, ap);
        syslog(level, "%s", buffer);
        free(buffer);
    }

    va_end(ap);
}