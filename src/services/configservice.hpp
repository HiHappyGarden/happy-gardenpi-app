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

#include "../utilities/object.hpp"
#include "../constants.hpp"
#include "../pods/configinfo.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        using std::make_shared;
        using std::string;

        /**
         * @brief Class for reading json configuration
         * 
         */
        class ConfigSerivce final : public Object
        {

            const string configFile;

            ConfigInfo::Ptr config;

        public:
            /**
             * @brief Construct a new Config Serivce object
             * 
             * @param configFile path of config file in json format
             */
            inline explicit ConfigSerivce(string &&configFile) noexcept : configFile(configFile), config(make_shared<ConfigInfo>())
            {
            }
            HGARDENPI_NO_COPY_NO_MOVE(ConfigSerivce)

            /**
             * @brief read configuration
             * 
             * @throw runtime_error if something is wrong
             */
            void read();

            /**
             * @brief Return the name of object
             * 
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }
        };
    }
}