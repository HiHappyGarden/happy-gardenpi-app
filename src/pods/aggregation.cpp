// MIT License
//
// Copyright (c) 2021. Happy GardenPI
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
//

//
// Created by Antonio Salsi on 19/07/21.
//

#include "aggregation.hpp"

#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

#define HGARDENPI_SET_SCHEDULE_FIELD(field) \
if (this->field < Schedule::NOT_SET) \
{ \
    ret += to_string(this->field); \
} \
else \
{ \
    ret += "*"; \
} \
ret += "/"; \


#define HGARDENPI_FROM_STRING_TO_UINT(field) \
 field = s != "*" ? stoi(s) : NOT_SET; break;

namespace hgardenpi
{

    inline namespace v1
    {


        void Aggregation::Schedule::set(uint8_t hour, uint8_t minute, uint8_t days)
        {
            if (hour > 23 && hour != Aggregation::Schedule::NOT_SET)
            {
                throw runtime_error("hour value not in 0-59 range");
            }

            if (minute > 59 && minute != Aggregation::Schedule::NOT_SET)
            {
                throw runtime_error("minute value not in 0-59 range");
            }

            if (days > 0x7F && days != Aggregation::Schedule::NOT_SET)
            {
                throw runtime_error("days value not in 0x01-0x7F range");
            }

            this->hour = hour;
            this->minute = minute;
            this->days = days;
        }

        void Aggregation::Schedule::set(const string &scheduleFormat)
        {
            stringstream ss(scheduleFormat);
            string s;

            uint8_t index = 0;
            while (getline(ss, s, '/')) {
                switch (index) // NOLINT(hicpp-multiway-paths-covered)
                {
                    case 0: HGARDENPI_FROM_STRING_TO_UINT(hour)
                    case 1: HGARDENPI_FROM_STRING_TO_UINT(minute)
                    case 2: HGARDENPI_FROM_STRING_TO_UINT(days)
                }
                index++;
            }
            set(hour, minute, days);
        }


        string Aggregation::Schedule::get() const noexcept
        {
            string ret;
            HGARDENPI_SET_SCHEDULE_FIELD(hour)
            HGARDENPI_SET_SCHEDULE_FIELD(minute)
            HGARDENPI_SET_SCHEDULE_FIELD(days)

            ret = move(ret.substr(0, ret.size() - 1));

            return ret;
        }

    }
}