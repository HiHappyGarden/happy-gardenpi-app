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

#include "../config.h"

using namespace SQLite;

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief Creation database queries
         */
        static const char *dbSql[] = {
                "CREATE TABLE `metadata` (`version` INTEGER NOT NULL DEFAULT 0, `wifiConfigured` INTEGER NOT NULL DEFAULT 0);",
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
                "\t\"relay_number\"\tINTEGER,\n"
                "\t\"watering_time\"\tINTEGER,\n"
                "\t\"weight\"\tINTEGER DEFAULT 1,\n"
                "\t\"status\"\tINTEGER DEFAULT 1,\n"
                "\t\"id_aggregation\"\tINTEGER,\n"
                "\tFOREIGN KEY(\"id_aggregation\") REFERENCES \"aggregations\"(\"id\"),\n"
                "\tPRIMARY KEY(\"id\" AUTOINCREMENT)\n"
                ")",

#if HGARDENPI_TEST > 0
                //test
                "INSERT INTO aggregations VALUES (1, \"Test station\", 1, \"10/30/127\",  \"2021/01/01\", \"2021/12/31\", 1, 1);",
                "INSERT INTO stations  VALUES (1, \"Station 1\", \"Giardino nord\", 6, 40, 10, 1, 1);",
                "INSERT INTO stations  VALUES (2, \"Station 2\", \"Giardino sud\", 10, 15, 20, 1, 1);",
                "INSERT INTO stations  VALUES (3, \"Station 3\", \"Giardino est\", 11, 30, 30, 1, 1);",
                "INSERT INTO stations  VALUES (4, \"Station 4\", \"Giardino ovest\", 31, 10, 40, 1, 1);",
#endif
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


        Metadata DBGetMetadata(const Database &database)
        {
            Metadata ret{0, false};

            auto &db = const_cast<Database &>(database);

            // Compile a SQL query, containing one parameter (index 1)
            Statement query(db, "SELECT * FROM metadata");

            // Loop to execute the query step by step, to get rows of result
            while (query.executeStep())
            {
                std::get<0>(ret) = query.getColumn(0);
                std::get<1>(ret) = query.getColumn(1).getInt();
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

