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

#include <string>

#include "button.hpp"
#include <mutex>
#include <functional>

namespace hgardenpi
{
    inline namespace v1
    {

        using std::string;
        using std::mutex;
        using std::function;

        /**
         * @brief Representation concrete of button
         */
        class ButtonConcrete final : public Button
        {
        public:

            using Callback = function<void()>;

            enum GPIOPinDisplay : int
            {
                PIN = 8,
            };

        private:
            mutex m;

            Callback callback;
        public:
            explicit ButtonConcrete(int lcdRS) noexcept;
            HGARDENPI_NO_COPY_NO_MOVE(ButtonConcrete)

            /**
             * @brief Set callback triggered when button pull down
             * @param callback called when button pull down
             */
            void setCallback(Callback callback) noexcept;

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
