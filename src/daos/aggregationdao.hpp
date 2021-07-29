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

#pragma once

#include "dao.hpp"
#include "../pods/aggregation.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief AggregationDAO dao
         */
        class AggregationDAO : public DAO<Aggregation>
        {
        public:

            static const constexpr char *TABLE = "aggregations";

            inline explicit AggregationDAO(const string &dbFile) noexcept : DAO(dbFile)
            {

            }
            HGARDENPI_NO_COPY_NO_MOVE(AggregationDAO)

            /**
             * @brief Fill a Aggregation pod
             * @param statement of an already execute query
             * @return aggregation pod
             * @throws exception when occur wrong access field
             */
            [[nodiscard]] Aggregation::Ptr fill(const Statement &statement) const override;

            /**
             * @brief insert Aggregation pod
             * @param ptr to pod
             * @throws exception when occur wrong access field
             */
            void insert(const Aggregation::Ptr &ptr) const override;

            /**
             * @brief update Aggregation pod
             * @param ptr to pod
             * @throws exception when occur wrong access field
             */
            void update(const Aggregation::Ptr &ptr) const override;

            /**
             * Retrieve list of Aggregation
             * @param status filter by status
             * @return list of aggregation
             * @throws exception when query is wrong
             */
            [[nodiscard]] Aggregations getList(Status status = Status::ACTIVE) const;

            /**
             * @brief Return the name of object
             *
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }
        };

    }
}