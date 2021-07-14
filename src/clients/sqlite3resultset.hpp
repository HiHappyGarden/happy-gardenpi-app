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

#include <map>

#include <sqlite3.h>
#include "../utilities/variant.hpp"
#include "resultset.hpp"

namespace hgardenpi
{


    inline namespace v1
    {

        class SQLIte3Database;

        class SQLite3ResultSet final : public virtual Object, public ResultSet
        {
            const SQLIte3Database *database;
            const string query;
            sqlite3_stmt *stmt;

            map <string, pair<int, int>> columns; //posizione e tipo di colonna

            bool isOpen = true;
            int status = SQLITE_BUSY;
            bool ready = false;

            friend SQLIte3Database;
        private:
            SQLite3ResultSet(const SQLIte3Database *, const string &, const vector <Variant> & = {}, bool = true);

        public:
            inline ~SQLite3ResultSet() noexcept
            { close(); }

            SQLite3ResultSet(const SQLite3ResultSet &) = delete;

            SQLite3ResultSet &operator=(const SQLite3ResultSet &) = delete;

            SQLite3ResultSet(SQLite3ResultSet &&) = delete;

            SQLite3ResultSet &operator=(SQLite3ResultSet &&) = delete;

            void close() noexcept override;

            bool next() noexcept override;

            void reset() noexcept override;

            const ResultSet *get(int) noexcept override;

            string getString(const string &) const override;

            int getInt(const string &) const override;

            inline long getLong(const string &column) const override
            { return static_cast<int>(getInt(column)); }

            float getFloat(const string &) const override;

            inline double getDouble(const string &column) const override
            { return static_cast<double>(getFloat(column)); }

            inline bool getBoolean(const string &column) const override
            { return getInt(column) == 1; }

            ByteArray getBlob(const string &) const override;

            string toString() const override;
        };

    }

}

