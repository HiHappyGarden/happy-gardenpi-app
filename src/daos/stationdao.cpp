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
// Created by Antonio Salsi on 18/07/21.
//

#include "stationdao.hpp"

using namespace hgardenpi::v1;

Station::Ptr StationDAO::fill(const SQLite::Statement &statement) const
{
    auto &&query = const_cast<Statement &>(statement);
    Station::Ptr ret = std::make_shared<Station>();
    ret->id = query.getColumn("id");
    ret->name = query.getColumn("name").getString();
    ret->description = query.getColumn("description").getString();
    ret->wateringTime = query.getColumn("watering_time");
    ret->weight = query.getColumn("weight");
    ret->status = static_cast<Status>(query.getColumn("status").getUInt());
    return ret;
}

void StationDAO::insert(const Station::Ptr &ptr) const
{
    Database database(dbFile, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    Transaction transaction(database);

    Statement query(database, "INSERT INTO stations (name, description, relay_number, watering_time, status, weight) VALUES (?, ?, ?, ?, ?, ?)");

    query.bind(1, ptr->name);
    query.bind(2, ptr->description);
    query.bind(3, ptr->relayNumber);
    query.bind(4, ptr->wateringTime);
    query.bind(5, ptr->weight);
    query.bind(6, static_cast<int>(ptr->status));


    query.executeStep();

    transaction.commit();
}

void StationDAO::update(const Station::Ptr &ptr) const
{
    Database database(dbFile, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    Transaction transaction(database);

    Statement query(database, "UPDATE aggregations SET name = ?, description = ?, relay_number = ?, watering_time = ?, weight = ?, status = ? WHERE id = ?");

    query.bind(1, ptr->name);
    query.bind(2, ptr->description);
    query.bind(3, ptr->relayNumber);
    query.bind(4, ptr->wateringTime);
    query.bind(5, ptr->weight);
    query.bind(6, static_cast<int>(ptr->status));
    query.bind(7, ptr->id);

    query.executeStep();

    transaction.commit();
}

Stations StationDAO::getList(const Aggregation::Ptr &ptr, Status status) const
{
    Stations ret;

    Database database(dbFile, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

    Transaction transaction(database);

    Statement query(database, "SELECT * FROM stations WHERE status = ? AND id_aggregation = ? ORDER BY weight");

    query.bind(1, static_cast<uint8_t>(status));
    query.bind(2, ptr->id);

    while (query.executeStep())
    {
        ret.push_back(move(fill(query)));
    }

    transaction.commit();

    return  ret;
}
