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

#include "sqlite3resultset.hpp"
#include "sqlite3database.hpp"

#define DB_GET_DATA(type, data) \
lock_guard<mutex> lg(database->m); \
if (!isOpen) { \
    throw runtime_error("database is closed"); \
}\
\
if(status != SQLITE_OK) { \
    throw runtime_error("wrong database state"); \
} \
\
if(!ready) { \
    throw runtime_error("result set not ready"); \
} \
\
try { \
    auto pair = columns.at(column); \
    \
    if (pair.second != type) { \
        throw runtime_error("wrong type"); \
    } \
\
    return data; \
} catch (const out_of_range &e) { \
    throw runtime_error(e.what()); \
}

namespace hgardenpi
{

    inline namespace v1
    {


        SQLite3ResultSet::SQLite3ResultSet(const SQLIte3Database *database, const string &query,
                                           const vector <Variant> &params, bool performStatement) : database(database),
                                                                                                    query(query),
                                                                                                    stmt(nullptr)
        {

            lock_guard <mutex> lg(database->m);

            status = sqlite3_prepare_v3(database->db, query.c_str(), -1, 0, &stmt, nullptr);
            if (status == SQLITE_OK)
            {

                int i = 1;
                for (auto &&param : params)
                {
                    switch (param.getType())
                    {
                        default:
                        case Variant::Type::STRING:
                            sqlite3_bind_text(stmt, i, param.toString().c_str(), -1, SQLITE_TRANSIENT);
                            break;
                        case Variant::Type::INT:
                            sqlite3_bind_int(stmt, i, param.toInt());
                            break;
                        case Variant::Type::LONG:
                            sqlite3_bind_int64(stmt, i, static_cast<sqlite3_int64>(param.toLong()));
                            break;
                        case Variant::Type::DOUBLE:
                            sqlite3_bind_double(stmt, i, param.toDouble());
                            break;
                        case Variant::Type::FLOAT:
                            sqlite3_bind_double(stmt, i, static_cast<double>(param.toFloat()));
                            break;
                        case Variant::Type::BOOL:
                            sqlite3_bind_int(stmt, i, param.toBoolean());
                            break;
                        case Variant::Type::BYTE_ARRAY:
                            sqlite3_bind_blob(stmt, i, param.toBlob().data(), static_cast<int>(param.toBlob().size()),
                                              SQLITE_STATIC);
                            break;
                    }
                    i++;
                }

                if (performStatement)
                {
                    bool first = true;
                    while (sqlite3_step(stmt) != SQLITE_DONE)
                    {
                        if (first)
                        {
                            for (int i = 0; i < sqlite3_column_count(stmt); i++)
                            {

                                int type = 0;
                                switch (sqlite3_column_type(stmt, i))
                                {
                                    case (SQLITE3_TEXT):
                                        type = SQLITE3_TEXT;
                                        break;
                                    case (SQLITE_INTEGER):
                                        type = SQLITE_INTEGER;
                                        break;
                                    case (SQLITE_FLOAT):
                                        type = SQLITE_FLOAT;
                                        break;
                                    case (SQLITE_BLOB):
                                        type = SQLITE_BLOB;
                                        break;
                                    default:
                                        break;
                                }
                                columns[sqlite3_column_name(stmt, i)] = pair < int, int > {i, type};
                            }
                            first = false;
                        }
                        count++;
                    }
                    sqlite3_reset(stmt);
                } else
                {
                    if (sqlite3_step(stmt) != SQLITE_ERROR)
                    {
                        count = 1;
                    } else
                    {
                        count = 0;
                    }
                }
            } else
            {
                close();
                throw runtime_error(__LINE__, __FILE__, sqlite3_errmsg(database->db));
            }
        }

#ifdef __clang__
#   pragma clang diagnostic pop
#endif

        void SQLite3ResultSet::close() noexcept
        {
            lock_guard <mutex> lg(database->m);
            if (!isOpen)
            {
                return;
            }
            if (database->resultsSetOpened > 0)
            {
                database->resultsSetOpened--;
            }
            sqlite3_finalize(stmt);
            isOpen = false;
        }

        bool SQLite3ResultSet::next() noexcept
        {
            lock_guard <mutex> lg(database->m);
            ready = true;
            if (status == SQLITE_OK)
            {
                return sqlite3_step(stmt) != SQLITE_DONE;
            } else
            {
                return false;
            }
        }

        void SQLite3ResultSet::reset() noexcept
        {
            lock_guard <mutex> lg(database->m);
            if (status != SQLITE_OK)
            {
                return;
            }
            sqlite3_reset(stmt);
        }

        const ResultSet *SQLite3ResultSet::get(int index) noexcept
        {
            lock_guard <mutex> lg(database->m);
            if (status != SQLITE_OK)
            {
                return nullptr;
            }
            int i = 0;
            while (sqlite3_step(stmt) != SQLITE_DONE)
            {
                if (i == index)
                {
                    return this;
                } else
                {
                    i++;
                }
            }
            return nullptr;
        }

        string SQLite3ResultSet::getString(const string &column) const
        {
            DB_GET_DATA(SQLITE3_TEXT, reinterpret_cast<const char *>(sqlite3_column_text(stmt, pair.first)))
        }

        int SQLite3ResultSet::getInt(const string &column) const
        {
            DB_GET_DATA(SQLITE_INTEGER, sqlite3_column_int(stmt, columns.at(column).first))
        }

        float SQLite3ResultSet::getFloat(const string &column) const
        {
            DB_GET_DATA(SQLITE_FLOAT, static_cast<float>(sqlite3_column_double(stmt, columns.at(column).first)))
        }

        ByteArray SQLite3ResultSet::getBlob(const string &column) const
        {
            lock_guard <mutex> lg(database->m);
            if (!isOpen)
            {
                throw runtime_error(__LINE__, __FILE__, "database is closed");
            }

            if (status != SQLITE_OK)
            {
                throw runtime_error(__LINE__, __FILE__, "wrong database state");
            }

            try
            {
                auto pair = columns.at(column);

                if (pair.second != SQLITE_BLOB)
                {
                    throw runtime_error(__LINE__, __FILE__, "wrong type");
                }
                int columnIndex = columns.at(column).first;

                auto &&ret = ByteArray(reinterpret_cast<const uint8_t *>(sqlite3_column_blob(stmt, columnIndex)),
                                       sqlite3_column_bytes(stmt, columnIndex));
                return ret;
            } catch (const out_of_range &e)
            {
                throw runtime_error(__LINE__, __FILE__, e.what());
            }
            return {};
        }

    }
}
