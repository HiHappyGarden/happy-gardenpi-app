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
#include <ostream>
#include <memory>

#include "../config.h"
#include "../interfaces/object.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        using std::ostream;
        using std::shared_ptr;
        using std::string;

        /**
         * @brief Data container for HW informations
         * 
         */
        struct ConfigInfo final : public Object
        {

            typedef shared_ptr<ConfigInfo> Ptr;

            string fileLock = HGARDENPI_FILE_LOCK_PATH;

            struct
            {
                string host = HGARDENPI_BROKER_HOST;
                uint16_t port = HGARDENPI_BROKER_PORT;
                string user = HGARDENPI_BROKER_USER;
                string passwd = HGARDENPI_BROKER_PASSWD;
            } broker;

            /**
             * @brief Database POD config
             */
            struct Database
            {
                string file = HGARDENPI_DB_FILE;
            } database;

            /**
             * @brief Return the representation off all data in POD
             *
             * @return std::string data inn json format
             */
            string toString() noexcept override;
        };

        ostream &operator<<(ostream &, ConfigInfo::Ptr const &);

        ostream &operator<<(ostream &, ConfigInfo const &);

    }
}
