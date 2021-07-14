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

#include "../interfaces/object.hpp"

#include <vector>
#include <memory>
using namespace std;


#define CPPC_ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

namespace hgardenpi {

inline namespace v1 {

template <typename T>
using SharedVector = vector<shared_ptr<T>>;

extern string hexToString(const uint8_t *, size_t, bool) noexcept;

class ByteArray : public Object, private vector<uint8_t> {

public:
    ByteArray() noexcept = default;
    ~ByteArray() noexcept = default;
    ByteArray(const uint8_t *, size_t);
    ByteArray(const char *); //non mettere explicit è voluto
    ByteArray(const string &) noexcept; //non mettere explicit è voluto


    string toString() const override;
    void add(const ByteArray &);

    string toBase64() const;
    inline static string toBase64(ByteArray &&byes) {
        return toBase64(byes);
    }
    static string toBase64(const ByteArray &);

    inline static ByteArray fromBase64(string &&byes) {
        return fromBase64(byes);
    }
    static ByteArray fromBase64(const string &);

    inline const uint8_t * toBytes() const noexcept
    {
        return static_cast<const uint8_t *>(&(*this)[0]);
    }

    inline string toHex(bool upperCase = true) const noexcept
    {
        return hexToString(toBytes(), size(), upperCase);
    }

    using vector<uint8_t>::size;
    using vector<uint8_t>::empty;
    using vector<uint8_t>::push_back;
    using vector<uint8_t>::data;
    using vector<uint8_t>::at;
    using vector<uint8_t>::operator[];
    using vector<uint8_t>::begin;
    using vector<uint8_t>::end;
    using vector<uint8_t>::clear;
    using vector<uint8_t>::allocator_type;

    friend bool operator==(const ByteArray&, const ByteArray&) noexcept;
    friend bool operator==(const ByteArray&, const char *) noexcept;
    friend bool operator==(const ByteArray&, const string &) noexcept;

    friend ByteArray operator+(const ByteArray&, const ByteArray&);

    friend string operator+(const string&, const ByteArray&);

    friend string operator+(const ByteArray&, const string&);

    friend ostream& operator<<(ostream&, const ByteArray &) noexcept;

};

}





