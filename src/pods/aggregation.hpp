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
// Created by Antonio Salsi on 17/07/21.
//

#pragma once

#include "station.hpp"
#include <date/date.h>

namespace hgardenpi
{

    inline namespace v1
    {

        using date::year_month_day;

        /**
         * @brief pod who describe an aggregation of station
         *
         * in this struct there are all information for scheduling a single or block of station
         */
        struct Aggregation
        {

            typedef shared_ptr<Aggregation> Ptr;

            /**
             * @brief id in db
             */
            uint id;
            /**
             * @brief brief description of aggregation
             */
            string description;
            /**
             * @brief brief manual check if the aggregation start automatically or manually by follow fields
             */
            bool manual = true;
            /**
             * @brief scheduling info
             */
            string schedule;
            /**
             * @brief start scheduling period if enhanced
             */
            year_month_day *start = nullptr;
            /**
             * @brief end scheduling period if enhanced
             */
            year_month_day *end = nullptr;
            /**
             * @brief status of station
             */
            Status status;
            /**
             * @brief list of managed stations
             */
            Stations stations;
        };


    }
}