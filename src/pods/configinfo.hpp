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

            /**
             * @brief shared_ptr of ConfigInfo
             */
            typedef shared_ptr<ConfigInfo> Ptr;

            /**
             * @brief where store file lock
             */
            string fileLock = HGARDENPI_FILE_LOCK_PATH;

            /**
             * @brief config info about mqtt broker
             */
            struct
            {
                /**
                 * @brief broker address or ip
                 */
                string host = HGARDENPI_BROKER_HOST;
                /**
                 * @brief broker port
                 */
                uint16_t port = HGARDENPI_BROKER_PORT;
                /**
                 * @brief broker user name for auth
                 */
                string user = HGARDENPI_BROKER_USER;
                /**
                 * @brief broker password for auth
                 */
                string passwd = HGARDENPI_BROKER_PASSWD;
            } broker;

            /**
             * @brief Database POD config
             */
            struct Database
            {
                /**
                 * @brief where store database file
                 */
                string file = HGARDENPI_DB_FILE;
            }
            /**
             * @brief Instance of database config
             */
            database;

            /**
             * @brief Return the representation off all data in POD
             * @return std::string data inn json format
             */
            string toString() noexcept override;
        };

        /**
         * Convenience operator overload for compatibility whit iostream
         * @return iostream reference
         */
        ostream &operator<<(ostream &, ConfigInfo::Ptr const &);

        /**
         * Convenience operator overload for compatibility whit iostream
         * @return iostream reference
         */
        ostream &operator<<(ostream &, ConfigInfo const &);

    }
}
