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

#include <memory>

#include "utilities/singleton.hpp"
#include "services/deviceservice.hpp"
#include "clients/mqttclientlocalsub.hpp"
//#include "interfaces/mqttclient.hpp"

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

            friend void initialize();
            friend void start();

            unique_ptr<LockService> lockService;
            DeviceInfo::Ptr deviceInfo;
            MQTTClient::Ptr mqttClient;

        public:
            Globals() noexcept;
            ~Globals() noexcept;
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
            inline const DeviceInfo *getDeviceInfo() const noexcept
            {
                return deviceInfo.get();
            }

            /**
              * @brief Get the Logk Serice object
              * 
              * @return const LockService* 
              */
            inline const LockService *getLogkSerice() const noexcept
            {
                return lockService.get();
            }

            /**
             * @brief Set the Mqtt Client object
             * 
             * @param mqttClient 
             */
            inline void settMqttClient(const MQTTClient::Ptr &mqttClient) noexcept
            {
                this->mqttClient = mqttClient;
            }

            /**
             * @brief Get the Mqtt Client object
             * 
             * @return const MQTTClient* 
             */
            inline const MQTTClient *getMqttClient() const noexcept
            {
                return mqttClient.get();
            }

            // /**
            //  * @brief Set the Run object
            //  *
            //  * @param run
            //  */
            // inline void setRun(bool run) noexcept
            // {
            //     this->run = run;
            // }

            // /**
            //  * @brief Get the Run object
            //  *
            //  * @return true run loop
            //  * @return false stop loop
            //  */
            // inline bool getRun() const noexcept
            // {
            //     return run;
            // }
        };

        /**
         * @brief Before all call this functiuon fo inilialize the project
         * 
         * @exception runtime_error when hardware requisites mismatch
         */
        void initialize();

        /**
         * @brief Start main look and scheduler
         * 
         */
        void start();

    }
}