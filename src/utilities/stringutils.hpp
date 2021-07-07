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
         * @param strToFind string to find
         * @return true if it's contained
         * @return false in otherwise
         */
        [[maybe_unused]] inline bool stringContain(const string &str, const string &strToFind) noexcept
        {
            return str.find(strToFind) != string::npos;
        }

        /**
         * @brief Check if into a string is contains another string
         * 
         * @param str string
         * @param strToFind string to find
         * @return true if it's contained
         * @return false in otherwise
         */
        [[maybe_unused]] inline bool stringContain(const string &str, const string &&strToFind) noexcept
        {
            return stringContain(str, strToFind);
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
        [[maybe_unused]] [[maybe_unused]] bool stringReplace(string &str, string &&from, string &&to) noexcept;

        /**
         * @brief Replace a string into another string many time
         * 
         * @param str source string
         * @param from token to replace
         * @param to replacing
         */
        [[maybe_unused]] void stringReplaceAll(string &str, string &&from, string &&to) noexcept;

        /**
         * @brief Convert string to byte array
         * 
         * @param str source
         * @return pair<uint8_t *, size_t> array converted
         */
        [[maybe_unused]] inline pair<uint8_t *, size_t> stringToBytes(string str) noexcept
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
        [[maybe_unused]] string stringBytesToString(const uint8_t *bytes, size_t size) noexcept;

        /**
         * @brief Convert byte array to string
         * 
         * @param bytes source
         * @return string convderted
         */
        [[maybe_unused]] inline string stringBytesToString(const pair<uint8_t *, size_t> &bytes) noexcept
        {
            return stringBytesToString(bytes.first, bytes.second);
        }

        /**
         * @brief Convert byte array to string
         * 
         * @param bytes source
         * @return string convderted
         */
        [[maybe_unused]] inline string stringBytesToString(const pair<uint8_t *, size_t> &&bytes) noexcept
        {
            return stringBytesToString(bytes.first, bytes.second);
        }

        /**
         * @brief Convert byte array to hex string
         * 
         * @param bytes source
         * @param size numer of bite to convert
         * @param upperCase force uppercase
         * @return string convderted
         */
        [[maybe_unused]] [[maybe_unused]] string stringHexToString(const uint8_t *bytes, size_t size, bool upperCase = true) noexcept; //

        /**
         * @brief Convert string to lower case
         * 
         * @param data source 
         * @return const string& self reference string in lower case
         */
        [[maybe_unused]] inline const string &stringToLowerCase(string &data) noexcept
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
        [[maybe_unused]] inline const string &stringToUpperCase(string &data) noexcept
        {
            transform(data.begin(), data.end(), data.begin(), ::toupper);
            return data;
        }

        /**
         * @brief Return strin from token index to end or size limit
         * 
         * @param src string source
         * @param token string reference 
         * @param offset to start
         * @return string new string
         */
        [[maybe_unused]] string stringRight(const string &src, const string &token, size_t offset = 0) noexcept;

        /**
         * @brief Return strin from token index to end or size limit
         * 
         * @param src string source
         * @param token string reference 
         * @param offset to start
         * @return string new string
         */
        [[maybe_unused]] inline string stringRight(const string &src, const string &&token, size_t offset = 0) noexcept
        {
            return stringRight(src, token, offset);
        }

    }
}