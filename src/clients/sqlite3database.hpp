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

#include <sqlite3.h>
#include "database.hpp"
#include "../services/logservice.hpp"



namespace hgardenpi {

inline namespace v1 {

class SQLite3ResultSet;
class DatabaseFactory;

class SQLIte3Database final : public virtual Object, public Database {
    
    const string file;
    sqlite3 *db = nullptr;
    const char ** createDB;
    UpdateDBVersion updateDBVersion = nullptr;
    LogWriter logWriter  = nullptr;
    
    mutable int resultsSetOpened = 0;
    mutable mutex m;
    
    friend DatabaseFactory;
    friend SQLite3ResultSet;
private:
    inline explicit SQLIte3Database(string file, const char ** createDB = nullptr, UpdateDBVersion updateDBVersion = nullptr, LogWriter logWriter = nullptr) noexcept : file(move(file)), createDB(createDB), updateDBVersion(move(updateDBVersion)), logWriter(move(logWriter)) {}
    ~SQLIte3Database() noexcept;
    
    void exec(const string &);
    
public:
    SQLIte3Database(const SQLIte3Database&) = delete;
    SQLIte3Database& operator=(const SQLIte3Database &) = delete;
    SQLIte3Database(SQLIte3Database&&) = delete;
    SQLIte3Database& operator=(SQLIte3Database&&) = delete;
    
    inline string toString() const override { return "db:" + file; }
    
    void open()  override;
    void close()  override;
    
    SPResultSet executeQuery(const string &, const vector<Variant> & = {}) const  override;
    
    bool execute(const string &, const vector<Variant> & = {}) const override;
    
};
}

}


