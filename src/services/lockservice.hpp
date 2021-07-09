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

#include <sched.h>

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief LockService check if exist another instance of Happy GardenPI
         * 
         */
        class LockService
        {

        protected:
            pid_t pidInExecution = 0;

        public:
            /** 
            * @brief Try to get lock.
            * 
            * @throw runtime_error when is not possibe create lock file
            * @return true if another instance already run
            */
            virtual bool lock() noexcept = 0;

            /** 
            * @brief Release the lock obtained with lock().
            */
            virtual void release() const noexcept = 0;

            /**
             * @brief Get the Pid In Execution object
             * 
             * 
             * @return pid_t if any other instance already run this valie is greater than 0
             */
            [[maybe_unused]] [[nodiscard]] inline pid_t getPidInExecution() const noexcept
            {
                return pidInExecution;
            }
        };

    }
}