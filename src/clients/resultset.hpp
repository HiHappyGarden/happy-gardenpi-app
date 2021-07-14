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

#include "../utilities/types.h"
#include "../interfaces/object.h"

#define CPPC_RESULT_SET(rs) rs->get()

namespace hgardenpi
{

    inline namespace v1
    {

        typedef shared_ptr<class ResultSet> SPResultSet;

        class ResultSet : public virtual Object
        {
        protected:
            int count = -1;
        public:
            ~ResultSet() = default;

            using Object::toString;

            virtual void close() noexcept = 0;

            virtual bool next() noexcept = 0;

            virtual void reset() noexcept = 0;

            virtual const ResultSet *get(int index) noexcept = 0;

            virtual string getString(const string &) const = 0;

            virtual int getInt(const string &) const = 0;

            virtual long getLong(const string &) const = 0;

            virtual double getDouble(const string &) const = 0;

            virtual float getFloat(const string &) const = 0;

            virtual bool getBoolean(const string &) const = 0;

            virtual ByteArray getBlob(const string &) const = 0;


        public:
            inline const ResultSet *operator[](int index) noexcept
            {
                return get(index);
            }

            inline int getCount() const noexcept
            {
                return count;
            }

            inline bool empty() const noexcept
            {
                return count == 0;
            }

            inline virtual string getString(string &&column) const
            {
                return getString(column);
            }

            inline virtual int getInt(string &&column) const
            {
                return getInt(column);
            }

            inline virtual long getLong(string &&column) const
            {
                return getLong(column);
            }

            inline virtual double getDouble(string &&column) const
            {
                return getDouble(column);
            }

            inline virtual float getFloat(string &&column) const
            {
                return getFloat(column);
            }

            inline virtual bool getBoolean(string &&column) const
            {
                return getBoolean(column);
            }

            inline virtual ByteArray getBlob(string &&column) const
            {
                return getBlob(column);
            }
        };

    }
}
