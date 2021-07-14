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

#include "sqlite3database.hpp"

#include <fstream>

#include "sqlite3resultset.hpp"

#define CC_TABLE_METADATA "CREATE TABLE metadata (version INTEGER)"
#define CC_TABLE_METADATA_SET_VERSION_2 "INSERT INTO metadata VALUES (1)"
#define CC_TABLE_METADATA_GET_VERSION_2 "SELECT * FROM metadata"

using hgardenpi::v1::SQLIte3Database;

SQLIte3Database::~SQLIte3Database() noexcept {
    close();
}

void SQLIte3Database::open() {
    lock_guard<mutex> lg(m);
    bool createMetadataTable = false;
    
    ifstream ifile(file);
    if (!ifile.good()) {
        ofstream ofile;
        ofile.open(file);
        ofile.close();
        createMetadataTable = true;
    }
    ifile.close();
    
    sqlite3_shutdown();
    sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    sqlite3_initialize();

    sqlite3_open_v2(file.c_str(), &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE|SQLITE_OPEN_FULLMUTEX, nullptr);
    if (db == NULL) {
        throw runtime_error(__LINE__, __FILE__, sqlite3_errmsg(db));
    }
    
    connectionOpen = true;
    
    if (createMetadataTable) {
        exec(CC_TABLE_METADATA);
        exec(CC_TABLE_METADATA_SET_VERSION_2);
        if (createDB) {
            const char **p = createDB;
            while (*p) {
                exec(*p);
                p++;
            }
        }
    }
    
    if (updateDBVersion) {
        sqlite3_stmt * stmt;
        if(sqlite3_prepare_v3(db, CC_TABLE_METADATA_GET_VERSION_2, -1, 0, &stmt, nullptr) != SQLITE_OK ){
            sqlite3_finalize(stmt);
            //impossibile leggere la versione del db
            updateDBVersion(this, -1);
            //throw runtime_error(__LINE__, __FILE__, sqlite3_errmsg(db));
        } else {
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                int ver = sqlite3_column_int(stmt, 0);
                updateDBVersion(this, ver);
            }
        }
        sqlite3_finalize(stmt);
    }
}

void SQLIte3Database::exec(const string &sql) {
    char *error = nullptr;
    if (logWriter) {
        logWriter(LogLevel::DBG, SOURCE, sql, {});
    }
    if(sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error) != SQLITE_OK ){
        string err = error;
        sqlite3_free(error);
        throw runtime_error(__LINE__, __FILE__, err.c_str());
    }
}

void SQLIte3Database::close() {
    lock_guard<mutex> lg(m);
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        connectionOpen = true;
 
        if (resultsSetOpened > 0) {
            throw runtime_error(__LINE__, __FILE__, "not all resultset are closed");
        }
    }
}

SPResultSet SQLIte3Database::executeQuery(const string &query, const vector<Variant> &params) const {
    resultsSetOpened++;
    if (logWriter) {
        logWriter(LogLevel::DBG, SOURCE, query, params);
    }
    return SPResultSet(new SQLite3ResultSet(this, query, params));
}

bool SQLIte3Database::execute(const string &query, const vector<Variant> &params) const  {
    if (logWriter) {
        logWriter(LogLevel::DBG, SOURCE, query, params);
    }
    return SQLite3ResultSet(this, query, params, false).getCount() > 0;
}

