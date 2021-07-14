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

#include "../interfaces/object.h"
#include "types.h"

namespace hgardenpi {

inline namespace v1 {

class Variant final : public virtual Object {
public:
    enum class Type {
        STRING,
        INT,
        LONG,
        DOUBLE,
        FLOAT,
        BOOL,
        BYTE_ARRAY
    };
    
protected:
    string value = "";
    ByteArray bytes;
    Type type;
    
    friend Variant operator+(const Variant&, const Variant&);
    friend Variant operator+(const Variant&, const char *) noexcept;
    friend Variant operator+(const Variant&, const string &) noexcept;
    friend Variant operator+(const Variant&, string &&) noexcept;
    friend Variant operator+(const Variant&, const bool&);
    friend Variant operator+(const Variant&, const float&);
    friend Variant operator+(const Variant&, const double&);
    friend Variant operator+(const Variant&, const long&);
    friend Variant operator+(const Variant&, const int&);
    
    friend Variant operator-(const Variant&, const Variant&);
    friend Variant operator-(const Variant&, const bool&);
    friend Variant operator-(const Variant&, const float&);
    friend Variant operator-(const Variant&, const double&);
    friend Variant operator-(const Variant&, const long&);
    friend Variant operator-(const Variant&, const int&);
    
    friend Variant operator/(const Variant&, const Variant&);
    friend Variant operator/(const Variant&, const float&);
    friend Variant operator/(const Variant&, const double&);
    friend Variant operator/(const Variant&, const long&);
    friend Variant operator/(const Variant&, const int&);
    
    friend Variant operator*(const Variant&, const Variant&);
    friend Variant operator*(const Variant&, const float&);
    friend Variant operator*(const Variant&, const double&);
    friend Variant operator*(const Variant&, const long&);
    friend Variant operator*(const Variant&, const int&);
    
    friend bool operator==(const Variant&, const Variant&) noexcept;
    friend bool operator==(const Variant&, const char *) noexcept;
    friend bool operator==(const Variant&, const string &) noexcept;
    friend bool operator==(const Variant&, string &&) noexcept;
    friend bool operator==(const Variant&, const bool&);
    friend bool operator==(const Variant&, const float&);
    friend bool operator==(const Variant&, const double&);
    friend bool operator==(const Variant&, const long&);
    friend bool operator==(const Variant&, const int&);
    
    friend ostream& operator<<(ostream&, const Variant &) noexcept;
    
public:
    inline Variant(const string &value) noexcept : value(value), type(Type::STRING) {} // NOLINT(modernize-pass-by-value)
    inline Variant(string &&value) noexcept : value(value), type(Type::STRING) {}
    inline Variant(const char *value) noexcept : value(value), type(Type::STRING) {}
    inline Variant(int value) noexcept : value(to_string(value)), type(Type::INT) {}
    inline Variant(long value) noexcept : value(to_string(value)), type(Type::LONG) {}
    inline Variant(float value) noexcept : value(to_string(value)), type(Type::FLOAT) {}
    inline Variant(double value) noexcept : value(to_string(value)), type(Type::DOUBLE) {}
    inline Variant(bool value) noexcept : value(to_string(value)), type(Type::BOOL) {}
    inline Variant(const ByteArray &value) noexcept : bytes(value), type(Type::BYTE_ARRAY) {}
    inline Variant(ByteArray &&value) noexcept : bytes(value), type(Type::BYTE_ARRAY) {}
    
    inline string toString() const override {
        if (type == Type::BYTE_ARRAY) {
            return bytes.toString();
        }
        return value;
    }
    
    inline int toInt() const {
        if (type != Type::INT) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return stoi(value);
    }
    inline long toLong() const  {
        if (type != Type::LONG) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return stol(value);
    }
    inline double toDouble() const  {
        if (type != Type::DOUBLE) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return stod(value);
    }
    inline float toFloat() const  {
        if (type != Type::FLOAT) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return stof(value);
    }
    inline bool toBoolean() const  {
        if (type != Type::BOOL) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return value == "1";
    }
    inline const ByteArray & toBlob() const  {
        if (type != Type::BYTE_ARRAY) {
            throw runtime_error(__LINE__, __FILE__, "cast error");
        }
        return bytes;
    }
    
    inline const Type & getType() const noexcept {
        return type;
    }
};

typedef Variant var;
typedef const Variant val;

}

}


