// MIT License
//
// Copyright (c) $year. Happy GardenPI
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
// Created by Antonio Salsi on 14/08/21.
//

#include "relaymodule4channel.hpp"

#include <iostream>
#include <algorithm>
using namespace std;

#include <wiringPi.h>

#include "../config.h"

using hgardenpi::v1::RelayModule4Channel;

RelayModule4Channel::RelayModule4Channel(int in1, int in2, int in3, int in4) noexcept : in1(in1), in2(in2), in3(in3), in4(in4)
{
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
}

hgardenpi::v1::RelayModule4Channel::~RelayModule4Channel()
{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}


void RelayModule4Channel::setRelay(const Station::Ptr &ptr, bool status) const
{
    if (find(begin(wiringPiPinAll), end(wiringPiPinAll), ptr->relayNumber) == end(wiringPiPinAll))
    {
        throw runtime_error("pin: " + to_string(ptr->relayNumber) + " not allowed");
    }

#if HGARDENPI_TEST > 1
    digitalWrite(IN1, status);
#else
    digitalWrite(ptr->relayNumber, !status);
#endif
}



