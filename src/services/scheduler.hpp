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


#include "../interfaces/startable.hpp"
#include "../pods/aggregation.hpp"
#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief Scheduler for scheduing acrion, inside of this run a loop every one second
         * 
         */
        class Scheduler : public Startable, public Object
        {

        public:
            Scheduler() = default;
            virtual ~Scheduler() = default;

            /**
             * Insert into scheduler an aggregation of station
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            virtual void schedule(const Aggregation::Ptr &ptr) = 0;

            /**
             * Shot one time an aggregation of station triggered from user
             * @param ptr of aggregation
             * @throw runtime_error when there some problem in aggregation
             */
            virtual void shot(const Aggregation::Ptr &ptr) const = 0;

            /**
             * Shot one time an station triggered from user
             * @param ptr of station
             * @throw runtime_error when there some problem in station
             */
            virtual void shot(const Station::Ptr &ptr) const = 0;

        };

    }
}