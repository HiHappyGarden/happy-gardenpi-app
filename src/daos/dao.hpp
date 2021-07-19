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

#include <memory>
#include <string>
#include <SQLiteCpp/Statement.h>
#include <SQLiteCpp/Database.h>
#include <SQLiteCpp/Transaction.h>

#include "../interfaces/object.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        using std::shared_ptr;
        using std::string;
        using SQLite::Statement;
        using SQLite::Database;
        using SQLite::Transaction;

        /**
         * @brief base dao class
         */
        template<typename T>
        class DAO : public Object
        {
        protected:
            string dbFile;

            inline explicit DAO(const string &dbFile) noexcept : dbFile(dbFile)
            {

            }

        public:
            HGARDENPI_NO_COPY_NO_MOVE(DAO)

            /**
             * @brief fill a pod
             * @return pod filled
             */
            [[nodiscard]] virtual shared_ptr<T> fill(const Statement &) const = 0;

            /**
             * @brief insert a pod in db
             * @param pod to insert
             */
            virtual void insert(const shared_ptr<T> &) const = 0;

            /**
             * @brief update a pod in db
             * @param pod to update
             */
            virtual void update(const shared_ptr<T> &) const = 0;
        };

    }
}

