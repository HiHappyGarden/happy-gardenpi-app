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

#include "deviceservice.hpp"

#include <iostream>
#include <fstream>
using namespace std;

namespace hgardenpi
{
    inline namespace v1
    {

        extern inline bool stringConatin(const string &, const string &) noexcept;
        extern inline bool stringConatin(const string &, const string &&) noexcept;

        float getCPUTemperature()
        {
            string val;
            ifstream file("/sys/class/thermal/thermal_zone0/temp");
            file >> val;
            file.close();
            return stof(val) / 1000.0;
        }

        DeviceInfo::Ptr getDeviceInfo()
        {
            auto device = unique_ptr<DeviceInfo>(new (nothrow) DeviceInfo{"", "", "", ""});
            ifstream file("/proc/cpuinfo");
            string line;
            while (getline(file, line))
            {
                if (stringConatin(line, "Hardware"))
                {
                                }
                if (stringConatin(line, "Revision"))
                {
                }
                if (stringConatin(line, "Serial"))
                {
                }
                if (stringConatin(line, "Model"))
                {
                }
            }
            file.close();
            return device;
        }

    }
}