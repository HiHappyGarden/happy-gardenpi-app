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
#include "../pods/station.hpp"
#include "../pods/aggregation.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief Direct Access Object for Station POD
         */
        class StationDAO : public DAO<Station>
        {
        public:
            /**
             * Table name of this DAO
             */
            static inline const constexpr char *TABLE = "stations";

            inline explicit StationDAO(const string &dbFile) noexcept : DAO(dbFile)
            {
            }

            /**
             * @brief Fill a Station pod
             * @param statement of an already execute query
             * @return aggregation pod
             * @throws exception when occur wrong access field
             */
            [[nodiscard]] Station::Ptr fill(const Statement &statement) const override;

            /**
             * @brief insert Station pod
             * @param ptr to pod
             * @throws exception when occur wrong access field
             */
            void insert(const Station::Ptr &ptr) const override;

            /**
             * @brief update Station pod
             * @param ptr to pod
             * @throws exception when occur wrong access field
             */
            void update(const Station::Ptr &ptr) const override;

            /**
             * Get list of stations for aggregation
             * @param prt to aggregation
             * @return list of stations for aggregation
             * @exception runtime_error when hardware requisites mismatch
             */
            [[nodiscard]] Stations getList(const Aggregation::Ptr &ptr, Status status = Status::ACTIVE) const;

            /**
             * @brief Return the name of object
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }
        };

    }
}