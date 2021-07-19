// MIT License
//
// Copyright (c) $year. Happy GardenPI
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
// Created by Antonio Salsi on 18/07/21.
//

#include "aggregationdao.hpp"

using namespace hgardenpi::v1;

Aggregation::Ptr AggregationDAO::fill(const Statement &statement) const
{
    return shared_ptr<Aggregation>();
}

void AggregationDAO::insert(const Aggregation::Ptr &ptr) const
{
    Database database(dbFile, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    Transaction transaction(database);

    Statement   query(database, "INSERT INTO aggregation (description, manual, schedule, start, end, status) VALUES (?, ?, ?, ?, ?, ?)");
    // Bind the blob value to the first parameter of the SQL query
    query.bind(1, ptr->description);
    query.bind(2, ptr->manual);
    //query.bind(3, ptr->schedule);
    //query.bind(4, ptr->start);
    //query.bind(5, ptr->end);
    query.bind(6, static_cast<int>(ptr->status));

    transaction.commit();
}

void AggregationDAO::update(const Aggregation::Ptr &ptr) const
{

}
