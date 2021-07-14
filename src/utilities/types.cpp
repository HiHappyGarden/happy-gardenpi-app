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

#include "types.hpp"
//#include "cppcommons/crypto/base64.h"
//using ::cppcommons::crypto::base64::encode;
//using ::cppcommons::crypto::base64::decode;

namespace hgardenpi
{

    inline namespace v1
    {

        ByteArray::ByteArray(const uint8_t *bytes, size_t size)
        {
            if (!bytes)
            {
                throw runtime_error(__LINE__, __FILE__, "nullptr bytes");
            }

            const uint8_t *p = bytes;
            for (size_type i = 0; i < size; i++)
            {
                push_back(*p);
                p++;
            }
        }


        ByteArray::ByteArray(const char *c)
        {
            if (!c)
            {
                throw runtime_error(__LINE__, __FILE__, "nullptr");
            }

            const char *p = c;
            for (size_type i = 0; i < strlen(c); i++)
            {
                push_back(*p);
                p++;
            }
        }

        ByteArray::ByteArray(const string &buffer) noexcept
        {
            const char *p = buffer.c_str();
            for (size_type i = 0; i < buffer.size(); i++)
            {
                push_back(*p);
                p++;
            }
        }

        string ByteArray::toString() const
        {
            string ret;
            for (auto b : *this)
            {
                ret += b;
            }
            return ret;
        }

        void ByteArray::add(const ByteArray &bytes)
        {
            insert(end(), bytes.begin(), bytes.end());
        }

        string ByteArray::toBase64() const
        {
            return ByteArray::toBase64(*this);
        }

        string ByteArray::toBase64(const ByteArray &bytes)
        {
            return encode(bytes.data(), static_cast<int>(bytes.size()));
        }

        ByteArray ByteArray::fromBase64(const string &s)
        {
            return decode(s);
        }

        bool operator==(const ByteArray &a, const ByteArray &b) noexcept
        {
            if (a.size() != b.size())
            {
                return false;
            }
            return memcmp(a.data(), b.data(), sizeof(*a.data())) == 0;
        }

        bool operator==(const ByteArray &a, const char *b) noexcept
        {
            return a.toString() == b;
        }

        bool operator==(const ByteArray &a, const string &b) noexcept
        {
            return a.toString() == b;
        }

        ByteArray operator+(const ByteArray &a, const ByteArray &b)
        {
            auto ret = a;
            ret.add(b);
            return ret;
        }

        string operator+(const string &a, const ByteArray &b)
        {
            return a + b.toString();
        }

        string operator+(const ByteArray &a, const string &b)
        {
            return a.toString() + b;
        }

        ostream &operator<<(ostream &os, const ByteArray &ba) noexcept
        {
            os << const_cast<ByteArray &>(ba).toString();
            return os;
        }
    }
}

