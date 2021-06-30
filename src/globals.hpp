/*
MIT License

Copyright (c) 2021 Happy GardenPI

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <memory>

#include "utilities/singleton.hpp"
#include "services/deviceservice.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        using std::unique_ptr;

        class LockService;

        /**
         * @brief Singleton where are put global variable
         */
        class Globals final : public Singleton<Globals>
        {
            bool lockServicePassThrough = false;
            unique_ptr<LockService> lockService;

            friend void initialize();

            DeviceInfo::Ptr deviceInfo;

        public:
            Globals() noexcept;
            HGARDENPI_NO_COPY_NO_MOVE(Globals)

            /**
             * @brief Return the name of object
             * 
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }

            /**
             * @brief Get the actual device info
             * 
             * @return const DeviceInfo::Ptr& reference
             */
            const DeviceInfo::Ptr &getDeviceInfo() const noexcept
            {
                return deviceInfo;
            }
        };

        /**
         * @brief before all call this functiuon fo inilialize the project
         * 
         */
        void initialize();

    }
}