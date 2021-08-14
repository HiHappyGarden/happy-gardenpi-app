// MIT License
//
// Copyright (c) 2021. Happy GardenPI
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
//

//
// Created by Antonio Salsi on 17/07/21.
//
#pragma once

#include <cstdint>
#include <tuple>

namespace SQLite
{
    class Database;
}

namespace hgardenpi
{
    inline namespace v1
    {

        using SQLite::Database;

        /**
         * @brief definition of matadata type
         */
        typedef std::tuple<uint8_t, bool> Metadata;

        /**
         * @brief Default metedata table name
         */
        const inline constexpr char *DB_METADATA_TABLE = "metadata";

        /**
         * @brief Get version of database
         * @param database instance
         * @return version
         */
        Metadata DBGetMetadata(const Database &database);

        /**
         * @brief Update or create a database structure
         * @param database instance
         * @param actualVersion actual version database if pass 0 the database structure will be create
         */
        void DBUpdate(const Database &database, uint8_t actualVersion = 0);

    }
}


