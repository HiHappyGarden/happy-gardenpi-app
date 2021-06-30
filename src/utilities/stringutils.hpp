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

#include <string>
#include <utility>
#include <algorithm>

namespace hgardenpi
{
    inline namespace v1
    {

        using std::pair;
        using std::string;
        using std::transform;

        /**
         * @brief Check if into a string is contains another string
         * 
         * @param str string
         * @param toFind string to find
         * @return true if it's contained
         * @return false in otherwise
         */
        inline bool stringConatin(const string &str, const string &toFind) noexcept
        {
            return str.find(toFind) != string::npos;
        }

        /**
         * @brief Check if into a string is contains another string
         * 
         * @param str string
         * @param toFind string to find
         * @return true if it's contained
         * @return false in otherwise
         */
        inline bool stringConatin(const string &str, const string &&toFind) noexcept
        {
            return stringConatin(str, toFind);
        }

        /**
         * @brief Replace once a string into another string
         * 
         * @param str source string
         * @param from token to replace
         * @param to replacing
         * @return true if it's replaced
         * @return false if doesn't replaced
         */
        bool replace(string &str, string &&from, string &&to) noexcept;

        /**
         * @brief Replace a string into another string many time
         * 
         * @param str source string
         * @param from token to replace
         * @param to replacing
         */
        void replaceAll(string &str, string &&from, string &&to) noexcept;

        /**
         * @brief Convert string to byte array
         * 
         * @param str source
         * @return pair<uint8_t *, size_t> array converted
         */
        inline pair<uint8_t *, size_t> stringToBytes(string str) noexcept
        {
            return pair<uint8_t *, size_t>{reinterpret_cast<uint8_t *>(&str[0]), str.size()};
        }

        /**
         * @brief Convert byte array to string
         * 
         * @param bytes source
         * @param size numer of bite to convert
         * @return string convderted
         */
        string bytesToString(const uint8_t *bytes, size_t size) noexcept;

        /**
         * @brief Convert byte array to string
         * 
         * @param bytes source
         * @return string convderted
         */
        inline string bytesToString(const pair<uint8_t *, size_t> &bytes) noexcept
        {
            return bytesToString(bytes.first, bytes.second);
        }

        /**
         * @brief Convert byte array to string
         * 
         * @param bytes source
         * @return string convderted
         */
        inline string bytesToString(const pair<uint8_t *, size_t> &&bytes) noexcept
        {
            return bytesToString(bytes.first, bytes.second);
        }

        /**
         * @brief Convert byte array to hex string
         * 
         * @param bytes source
         * @param size numer of bite to convert
         * @param upperCase force uppercase
         * @return string convderted
         */
        string hexToString(const uint8_t *bytes, size_t size, bool upperCase = true) noexcept; //

        /**
         * @brief Convert string to lower case
         * 
         * @param data source 
         * @return const string& self reference string in lower case
         */
        inline const string &toLowerCase(string &data) noexcept
        {
            transform(data.begin(), data.end(), data.begin(), ::tolower);
            return data;
        }

        /**
         * @brief Convert string to upper case
         * 
         * @param data source 
         * @return const string& self reference string in upper case
         */
        inline const string &toUpperCase(string &data) noexcept
        {
            transform(data.begin(), data.end(), data.begin(), ::toupper);
            return data;
        }
    }
}