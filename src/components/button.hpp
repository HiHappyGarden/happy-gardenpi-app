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
// Created by Antonio Salsi on 29/07/21.
//

#pragma once

#include <functional>


#include "../interfaces/object.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        /**
         * @brief Abstract button
         */
        class Button : public Object
        {
        public:
            Button() = default;
            virtual ~Button() = default;
            HGARDENPI_NO_COPY_NO_MOVE(Button)

            /**
             *  @brief Callback function
             */
            using OnClick = std::function<void()>;

            /**
             * @brief Set onClick triggered when button pull down
             * @param onClick called when button pull down
             */
            virtual void setOnClick(OnClick onClick) const noexcept = 0;

            /**
             * @brief Set onClick only for internal management triggered when button pull down
             * @param onClick called when button pull down
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void setInternalOnClick(OnClick onClick) const = 0;

        };
    }
}
