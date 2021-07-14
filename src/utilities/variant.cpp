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

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma ide diagnostic ignored "bugprone-narrowing-conversions"
#endif

#include "variant.hpp"

namespace hgardenpi {

inline namespace v1 {

Variant operator+(const Variant& a, const Variant& b) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value + b.value;
        case Variant::Type::BOOL: return (a.value == "1") + (b.value == "1");
        case Variant::Type::FLOAT: return stof(a.value) + stof(b.value);
        case Variant::Type::DOUBLE: return stod(a.value) + stod(b.value);
        case Variant::Type::LONG: return stol(a.value) + stol(b.value);
        case Variant::Type::INT: return stoi(a.value) + stoi(b.value);
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator+(const Variant&a, const char *s) noexcept {
    return  a.value + string(s);
}

Variant operator+(const Variant&a, const string &s) noexcept {
    return a.value + s;
}

Variant operator+(const Variant&a, string &&s) noexcept {
    return a.value + s;
}


Variant operator+(const Variant& a, const bool& b) {
    switch (a.type) {
        case Variant::Type::BOOL:
            return (a.value == "1") + b;
        default:
            throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator+(const Variant& a, const float& f) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value + f;
        case Variant::Type::FLOAT: return stof(a.value) + f;
        case Variant::Type::DOUBLE: return stod(a.value) + static_cast<double>(f);
        case Variant::Type::LONG: return stol(a.value) + f;
        case Variant::Type::INT: return stoi(a.value) + f;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator+(const Variant& a, const double& d) {
    switch (a.type) {
        case Variant::Type::STRING:
            return a.value + d;
        case Variant::Type::FLOAT:
            return stof(a.value) + static_cast<float>(d);
        case Variant::Type::DOUBLE:
            return stod(a.value) + d;
        case Variant::Type::LONG:
            return stol(a.value) + d;
        case Variant::Type::INT:
            return stoi(a.value) + d;
        default:
            throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator+(const Variant& a, const long& l) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value + l;
        case Variant::Type::FLOAT: return stof(a.value) + l;
        case Variant::Type::DOUBLE: return stod(a.value) + l;
        case Variant::Type::LONG: return stol(a.value) + l;
        case Variant::Type::INT: return stoi(a.value) + l;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator+(const Variant& a, const int& i) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value + i;
        case Variant::Type::FLOAT: return stof(a.value) + i;
        case Variant::Type::DOUBLE: return stod(a.value) + i;
        case Variant::Type::LONG: return stol(a.value) + i;
        case Variant::Type::INT: return stoi(a.value) + i;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const Variant& b) {
    switch (a.type) {
        case Variant::Type::BOOL: return (a.value == "1") - (b.value == "1");
        case Variant::Type::FLOAT: return stof(a.value) - stof(b.value);
        case Variant::Type::DOUBLE: return stod(a.value) - stod(b.value);
        case Variant::Type::LONG: return stol(a.value) - stol(b.value);
        case Variant::Type::INT: return stoi(a.value) - stoi(b.value);
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const bool& b) {
    switch (a.type) {
        case Variant::Type::BOOL: return (a.value == "1") - b;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const float& f) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) - f;
        case Variant::Type::DOUBLE: return stod(a.value) - static_cast<double>(f);
        case Variant::Type::LONG: return stol(a.value) - f;
        case Variant::Type::INT: return stoi(a.value) - f;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const double& d) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) - static_cast<float>(d);
        case Variant::Type::DOUBLE: return stod(a.value) - d;
        case Variant::Type::LONG: return stol(a.value) - d;
        case Variant::Type::INT: return stoi(a.value) - d;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const long& l) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) - l;
        case Variant::Type::DOUBLE: return stod(a.value) - l;
        case Variant::Type::LONG: return stol(a.value) - l;
        case Variant::Type::INT: return stoi(a.value) - l;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator-(const Variant& a, const int& i) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value - i;
        case Variant::Type::FLOAT: return stof(a.value) - i;
        case Variant::Type::DOUBLE: return stod(a.value) - i;
        case Variant::Type::LONG: return stol(a.value) - i;
        case Variant::Type::INT: return stoi(a.value) - i;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator/(const Variant& a, const Variant& b) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) / stof(b.value);
        case Variant::Type::DOUBLE: return stod(a.value) / stod(b.value);
        case Variant::Type::LONG: return stol(a.value) / stol(b.value);
        case Variant::Type::INT:return stoi(a.value) / stoi(b.value);
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator/(const Variant& a, const float& f) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) / f;
        case Variant::Type::DOUBLE: return stod(a.value) / static_cast<double>(f);
        case Variant::Type::LONG: return stol(a.value) / f;
        case Variant::Type::INT: return stoi(a.value) / f;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator/(const Variant& a, const double& d) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) / static_cast<float>(d);
        case Variant::Type::DOUBLE: return stod(a.value) / d;
        case Variant::Type::LONG: return stol(a.value) / d;
        case Variant::Type::INT: return stoi(a.value) / d;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator/(const Variant& a, const long& l) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) / l;
        case Variant::Type::DOUBLE: return stod(a.value) / l;
        case Variant::Type::LONG: return stol(a.value) / l;
        case Variant::Type::INT: return stoi(a.value) / l;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator/(const Variant& a, const int& i) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value / i;
        case Variant::Type::FLOAT: return stof(a.value) / i;
        case Variant::Type::DOUBLE: return stod(a.value) / i;
        case Variant::Type::LONG: return stol(a.value) / i;
        case Variant::Type::INT: return stoi(a.value) / i;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator*(const Variant& a, const Variant& b) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) * stof(b.value);
        case Variant::Type::DOUBLE: return stod(a.value) * stod(b.value);
        case Variant::Type::LONG: return stol(a.value) * stol(b.value);
        case Variant::Type::INT: return stoi(a.value) * stoi(b.value);
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator*(const Variant& a, const float& f) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) * f;
        case Variant::Type::DOUBLE: return stod(a.value) * static_cast<double>(f);
        case Variant::Type::LONG: return stol(a.value) * f;
        case Variant::Type::INT: return stoi(a.value) * f;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator*(const Variant& a, const double& d) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) * static_cast<float>(d);
        case Variant::Type::DOUBLE: return stod(a.value) * d;
        case Variant::Type::LONG: return stol(a.value) * d;
        case Variant::Type::INT: return stoi(a.value) * d;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator*(const Variant& a, const long& l) {
    switch (a.type) {
        case Variant::Type::FLOAT: return stof(a.value) * l;
        case Variant::Type::DOUBLE: return stod(a.value) * l;
        case Variant::Type::LONG: return stol(a.value) * l;
        case Variant::Type::INT: return stoi(a.value) * l;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

Variant operator*(const Variant& a, const int& i) {
    switch (a.type) {
        case Variant::Type::STRING: return a.value * i;
        case Variant::Type::FLOAT: return stof(a.value) * i;
        case Variant::Type::DOUBLE: return stod(a.value) * i;
        case Variant::Type::LONG: return stol(a.value) * i;
        case Variant::Type::INT: return stoi(a.value) * i;
        default: throw runtime_error(__LINE__, __FILE__, "cast error");
    }
}

bool operator==(const Variant&a, const Variant&s) noexcept {
    return strcmp(a.toString().c_str(), s.toString().c_str()) == 0;
}

bool operator==(const Variant&a, const char *s) noexcept {
    return a.toString() == s;
}

bool operator==(const Variant&a, const string &s) noexcept {
    return a.toString() == s;
}

bool operator==(const Variant&a, string &&s) noexcept {
    return a.toString() == s;
}

bool operator==(const Variant&a, const bool&b) {
    return a.toBoolean() == b;
}

bool operator==(const Variant&a, const float&f) {
    return a.toFloat() == f;
}

bool operator==(const Variant&a, const double&d) {
    return a.toDouble() == d;
}

bool operator==(const Variant&a, const long&l) {
    return a.toLong() == l;
}

bool operator==(const Variant&a, const int&i) {
    return a.toInt() == i;
}

ostream& operator<<(ostream& os, const Variant &variant) noexcept {
    if (variant.type == Variant::Type::BYTE_ARRAY) {
        os << variant.bytes.toString();
    } else {
        os << variant.value;
    }
    return os;
}
}

}
#ifdef __clang__
#   pragma clang diagnostic pop
#endif
