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

#include "station.hpp"

namespace hgardenpi
{

    inline namespace v1
    {
        using std::to_string;

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
            #pragma pack(push,2)
            struct Schedule
            {
                static const constexpr uint8_t NOT_SET = 0xFF;

                uint8_t minute; //0 - 59
                uint8_t hour; //0 - 23
                uint8_t days; //byte contains day enabled


                /**
                 * @brief Set the scheduling
                 * @param minute valid value 0 - 59
                 * @param hour valid value 0 - 23
                 * @param days in bit format
                 * @exception runtime_error when some value in not valid
                 */
                void set(uint8_t minute, uint8_t hour, uint8_t days);

                /**
                 * @brief Set the scheduling
                 * @param scheduleFormat
                 */
                void set(const string &scheduleFormat);

                /**
                 * @brief Get the scheduling
                 * @return string form of schedule
                 */
                [[nodiscard]] string get() const noexcept;

            } schedule;
            #pragma pack(pop)

            /**
             * @brief start scheduling period if enhanced
             */
            string start;
            /**
             * @brief end scheduling period if enhanced
             */
            string end;
            /**
             * If true execute sequentially the station otherwise execute all station at the same time
             */
            bool sequential = true;
            /**
             * @brief status of station
             */
            Status status = Status::ACTIVE;
            /**
             * @brief list of managed stations
             */
            Stations stations;

            /**
             *
             * @return
             */
            [[nodiscard]] inline string toString() const noexcept
            {
                return "id:" + to_string(id) + " "
                + "description:" + description + " "
                + "manual:" + to_string(manual) + " "
                + "schedule:" + schedule.get() + " "
                + "start:" + start + " "
                + "end:" + end + " "
                + "sequential:" + to_string(sequential) + " "
                + "status:" + to_string(static_cast<uint8_t>(status)) + " ";
            }
        };

        typedef list<Aggregation::Ptr> Aggregations;

    }
}