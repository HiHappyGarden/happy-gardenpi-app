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

#include "cppcommons/database/resultset.h"
#include "../interfaces/object.hpp"
#include "cppcommons/variant.h"

namespace hgardenpi {

inline namespace v1 {

class Database : public virtual Object {
protected:
    bool connectionOpen = false;
public:
    static constexpr const char *SOURCE = "Database";

    using UpdateDBVersion = function<void(const Database *, int)>;
    
    virtual ~Database() = default;
    
    using Object::toString;
    
    virtual void open()  = 0;
    virtual void close()  = 0;
    virtual SPResultSet executeQuery(const string &, const vector<Variant> & = {}) const  = 0;
    virtual inline SPResultSet executeQuery(const string &sql, const vector<Variant> &&params = {})
    {
        return executeQuery(sql, params);
    }
    
    virtual bool execute(const string &, const vector<Variant> & = {}) const  = 0;
    virtual inline bool execute(const string &sql, const vector<Variant> &&params = {})
    {
        return execute(sql, params);
    }
    
    inline bool isConnectionOpen() const noexcept { return connectionOpen; }
};

}
}


