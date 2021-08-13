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
#include <string>
#include <memory>

namespace hgardenpi
{

    inline namespace v1
    {

        using std::shared_ptr;
        using std::string;

        /**
         * @brief Data container for HW information
         * 
         */
        struct DeviceInfo final
        {

            /**
             * @brief shared_ptr of DeviceInfo
             */
            typedef shared_ptr<DeviceInfo> Ptr;

            /**
             * @brief information about chip
             */
            string hardware;
            /**
             * @brief board revision
             */
            string revision;
            /**
             * @brief board serial
             */
            string serial;
            /**
             * @brief board model
             */
            string model;
            /**
             * @brief board number of cpu
             */
            uint cpu;
        };

    }
}