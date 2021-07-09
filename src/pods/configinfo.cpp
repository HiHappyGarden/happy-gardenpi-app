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

#include "configinfo.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        string ConfigInfo::toString() noexcept
        {
            string ret;

            ret = "{\n";
            ret += " fileLock: " + fileLock + "\n";
            ret += " broker: {\n";
            ret += "  host: " + broker.host + "\n";
            ret += "  port: " + std::to_string(broker.port) + "\n";
            ret += "  user: " + broker.user + "\n";
            ret += "  passwd: " + broker.passwd + "\n";
            ret += "}";
            return ret;
        }

        ostream &operator<<(std::ostream &os, ConfigInfo::Ptr const &ci)
        {
            os << const_cast<ConfigInfo::Ptr &>(ci)->toString();
            return os;
        }

        ostream &operator<<(std::ostream &os, ConfigInfo const &ci)
        {
            os << const_cast<ConfigInfo &>(ci).toString();
            return os;
        }
    }
}