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

#include <atomic>

#include "device.hpp"
#include "../interfaces/object.hpp"
#include "../components/display.hpp"
#include "../components/button.hpp"
#include "../components/relaymodule.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        /**
         * @brief Factory for management of peripherals devices
         * 
         */
        class DeviceConcrete final : public Device
        {
            mutable std::atomic_bool contrastDisplayClicked = false;

            mutable DeviceInfo::Ptr deviceInfo = nullptr;
            LogService *logService = nullptr;
            ThreadPool *threadPool = nullptr;

            Display *display = nullptr;
            Button *button = nullptr;
            const RelayModule *relayModule = nullptr;

        public:
            DeviceConcrete() = default;
            ~DeviceConcrete() noexcept override;

            /**
             * @brief Before all call this function fo initialize the project
             * 
             * @exception runtime_error when hardware requisites mismatch
             */
            void initialize() override;

            /**
             * @brief Start main look and scheduler
             * @exception runtime_error when hardware requisites mismatch
             */
            void start() override;

            /**
             * @brief Get CPU temperature
             * @exception throw exception when temp is not available
             * @return return degrees temperature
             */
            float getCPUTemperature() const override;

            /**
             * @brief Get Raspberry PI info
             * @exception throw exception when temp is not available
             * @return return Raspberry PI info
             */
            DeviceInfo::Ptr getInfo() const override;

            /**
             * @brief Set the Log Service object
             * @param logService logService instance
             */
            inline void setLogService(const LogService *logService) noexcept override
            {
                this->logService = const_cast<LogService *>(logService);
            }

            /**
             * @brief Set ThreadPool instance
             * @param threadPool instance
             */
            inline void setThreadPool(ThreadPool *threadPool) noexcept override
            {
                this->threadPool = threadPool;
            }

            /**
             * @brief Print on system display
             * @param txt to show
             * @param enableFormat enable center formation
             */
            void printOnDisplay(const string &txt, bool enableFormat) const noexcept override;

            /**
             * @brief Get display instance
             * @return display instance
             */
            inline Display *getDisplay() const noexcept override
            {
                return display;
            }

            /**
             * @brief Get button instance
             * @return button instance
             */
            inline Button *getButton() const noexcept override
            {
                return button;
            }

            /**
             * @brief Get relayModule instance
             * @return relayModule instance
             */
            inline const RelayModule *getRelayModule() const noexcept override
            {
                return relayModule;
            }

            /**
             * @brief Return the name of object
             *
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }

        private:

            /**
             * Show mac address and ip to display
             * @param run true is in execution the loop
             */
            void printOnDisplayStandardInfo() const noexcept;

            /**
             * Turn on the display contrast for some time
             * @param run true is in execution the loop
             * @param wait time of contrast display it's turned on
             */
            void turnOnContrastDisplayFor(const Time &&wait = Time::DISPLAY_CONTRAST_TICK) noexcept;

        };

    }
}