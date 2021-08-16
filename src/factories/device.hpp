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

#include <functional>

#include "../interfaces/loggable.hpp"
#include "../interfaces/initializable.hpp"
#include "../interfaces/startable.hpp"
#include "../interfaces/closeable.hpp"
#include "../interfaces/object.hpp"
#include "../pods/deviceinfo.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        class ThreadPool;
        class Display;
        class Button;
        class RelayModule;

        /**
         * @brief Factory for management of peripherals devices
         * 
         */
        class Device : public Loggable, public Initializable, public Startable, public Object
        {
        public:

            /**
             * @brief For splitting text on more rows
             */
            static inline const constexpr char DIVISOR = '|';

            Device() = default;
            virtual ~Device() = default;
            HGARDENPI_NO_COPY_NO_MOVE(Device)

            /**
             * @brief Get CPU temperature
             * @exception throw exception when temp is not available
             * @return return degrees temperature
             */
            [[nodiscard]] virtual float getCPUTemperature() const = 0;

            /**
             * @brief Get Raspberry PI info
             * @exception throw exception when temp is not available
             * @return return Raspberry PI info
             */
            [[nodiscard]] virtual DeviceInfo::Ptr getInfo() const = 0;


            /**
             * @brief Set ThreadPool instance
             * @param threadPool instance
             */
            virtual void setThreadPool(ThreadPool *threadPool) noexcept = 0;

            /**
             * @brief Print on system display
             * @param txt to show
             * @param enableFormat enable center formation
             */
            virtual void printOnDisplay(const string &txt, bool enableFormat) const noexcept = 0;

            /**
             * @brief Get display instance
             * @return display instance
             */
            virtual Display *getDisplay() const noexcept = 0;

            /**
             * @brief Get button instance
             * @return button instance
             */
            virtual Button *getButton() const noexcept = 0;

            /**
             * @brief Get relayModule instance
             * @return relayModule instance
             */
            virtual const RelayModule *getRelayModule() const noexcept = 0;
        };

    }
}