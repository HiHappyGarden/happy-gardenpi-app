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

#include "databaseutils.hpp"

#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Transaction.h>

using namespace SQLite;

namespace hgardenpi
{
    inline namespace v1
    {

        static const char *dbSql[] = {
                "CREATE TABLE `metadata` (`version` INTEGER NOT NULL DEFAULT 0, `connected` INTEGER NOT NULL DEFAULT 0);",
                "CREATE TABLE \"aggregations\" (\n"
                "\t\"id\"\tINTEGER NOT NULL UNIQUE,\n"
                "\t\"description\"\tTEXT,\n"
                "\t\"manual\"\tINTEGER NOT NULL DEFAULT 1,\n"
                "\t\"schedule\"\tTEXT,\n"
                "\t\"start\"\tTEXT,\n"
                "\t\"end\"\tTEXT,\n"
                "\t\"sequential\"\tINTEGER DEFAULT 1,\n"
                "\t\"status\"\tINTEGER DEFAULT 1,\n"
                "\tPRIMARY KEY(\"id\" AUTOINCREMENT)\n"
                ")",
                "CREATE TABLE \"stations\" (\n"
                "\t\"id\"\tINTEGER NOT NULL UNIQUE,\n"
                "\t\"name\"\tTEXT NOT NULL,\n"
                "\t\"description\"\tTEXT,\n"
                "\t\"status\"\tINTEGER DEFAULT 1,\n"
                "\t\"id_aggregation\"\tINTEGER,\n"
                "\tFOREIGN KEY(\"id_aggregation\") REFERENCES \"aggregations\"(\"id\"),\n"
                "\tPRIMARY KEY(\"id\" AUTOINCREMENT)\n"
                ")",
                nullptr
        };

        static void execute(Database &db, const char *sequence[])
        {
            const char **p = sequence;

            Transaction transaction(db);
            while (*p)
            {
                db.exec(*p);
                p++;
            }

            db.exec("INSERT INTO metadata VALUES (1, 1)");

            transaction.commit();
        }


        uint8_t DBGetVersion(const Database &database)
        {
            uint8_t ret = 0;

            auto &db = const_cast<Database &>(database);

            // Compile a SQL query, containing one parameter (index 1)
            Statement query(db, "SELECT * FROM metadata");

            // Loop to execute the query step by step, to get rows of result
            while (query.executeStep())
            {
                ret = query.getColumn(0);
            }

            return ret;
        }

        void DBUpdate(const Database &database, uint8_t actualVersion)
        {
            auto &db = const_cast<Database &>(database);
            if (actualVersion == 0)
            {
                execute(db, dbSql);
            }
        }
    }
}

