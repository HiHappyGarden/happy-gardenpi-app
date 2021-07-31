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
#include <iostream>

#include "../constants.hpp"
#include "mqttclient.hpp"

struct mosquitto;
struct mosquitto_message;

namespace hgardenpi
{
    inline namespace v1
    {

        /**
         * @brief MQTT Client for connect to mosquitto
         */
        class MQTTClientMosquitto final : public MQTTClient
        {

            static inline const constexpr uint16_t KEEP_ALIVE = 60;
            static inline const constexpr uint16_t PORT = 1883;

            const string topic;
            const string &user;
            const string &passwd;

            LogService *logService = nullptr;
            mosquitto *mosq = nullptr;
            string id;

        public:

            /**
             * @brief Construct a new MQTTClient object
             * 
             * @param serial client reference 
             * @param host address
             * @param user host
             * @param passwd host
             * @param port host
             * @param keepAlive keep alive connection timeout
             */
            MQTTClientMosquitto(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port = MQTTClientMosquitto::PORT, uint16_t keepAlive = MQTTClientMosquitto::KEEP_ALIVE);

            /**
             * @brief Construct a new MQTTClient object
             * 
             * @param serial client reference 
             * @param host address
             * @param user host
             * @param passwd host
             * @param port host
             * @param keepAlive keep alive connection timeout
             */
            MQTTClientMosquitto(const string &serial, const string &&host, const string &&user, const string &&passwd, uint16_t port = MQTTClientMosquitto::PORT, uint16_t keepAlive = MQTTClientMosquitto::KEEP_ALIVE) : MQTTClientMosquitto(serial, host, user, passwd, port, keepAlive)
            {
            }

            ~MQTTClientMosquitto() noexcept override;

            HGARDENPI_NO_COPY_NO_MOVE(MQTTClientMosquitto)

            /**
             * @brief start loop 
             * @param run check if the loops are in execution
             * @exception runtime_error when hardware requisites mismatch
             */
            void loop(volatile bool &run) override;

            /**
             * @brief Set the On Message OnClick object
             * 
             * @param onMessageCallback 
             */
            inline void setOnMessageCallback(MessageCallback &&onMessageCallback) noexcept override
            {
                this->onMessageCallback = onMessageCallback;
            }

            inline void setLogService(const LogService *logService) noexcept override
            {
                this->logService = const_cast<LogService *>(logService);
            }

        private:
            MessageCallback onMessageCallback = nullptr;
        };
    }
}