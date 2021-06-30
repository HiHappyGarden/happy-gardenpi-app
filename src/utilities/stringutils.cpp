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

#include "stringutils.hpp"

#include <iomanip>
using namespace std;

namespace hgardenpi
{
    inline namespace v1
    {
        string stringLeft() noexcept
        {
            return "";
        }

        bool stringReplace(string &str, string &&from, string &&to) noexcept
        {
            size_t startPos = str.find(from);
            if (startPos == string::npos)
                return false;
            str.replace(startPos, from.length(), to);
            return true;
        }

        void stringReplaceAll(string &str, string &&from, string &&to) noexcept
        {
            if (from.empty())
                return;
            size_t startPos = 0;
            while ((startPos = str.find(from, startPos)) != string::npos)
            {
                str.replace(startPos, from.length(), to);
                startPos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
            }
        }

        string stringBytesToString(const uint8_t *key, size_t size) noexcept
        {
            if (!key)
            {
                return "";
            }
            ostringstream convert;
            for (int a = 0; a < size; a++)
            {
                convert << key[a];
            }
            return convert.str();
        }

        string stringHexToString(const uint8_t *bytes, size_t size, bool upperCase) noexcept
        {
            ostringstream ret;
            for (int i = 0; i < size; i++)
            {
                ret << hex << setfill('0') << setw(2) << (upperCase ? uppercase : nouppercase) << (int)bytes[i];
            }
            return ret.str();
        }

    }
}