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

#include <hgardenpi-protocol/packages/package.hpp>
using namespace hgardenpi::protocol;

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
            const string &host;
            uint16_t port;
            uint16_t keepAlive;
            const string &user;
            const string &passwd;

            mosquitto *mosq = nullptr;

            string id;

            bool initalizated = false;
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
            inline MQTTClientMosquitto(const string &serial, const string &&host, const string &&user, const string &&passwd, uint16_t port = MQTTClientMosquitto::PORT, uint16_t keepAlive = MQTTClientMosquitto::KEEP_ALIVE) : MQTTClientMosquitto(serial, host, user, passwd, port, keepAlive)
            {
            }

            ~MQTTClientMosquitto() noexcept override;

            /**
             * @brief start loop, place on the end of engine class
             * @exception runtime_error when something goes wrong
             */
            void loop() override;

            /**
             * @brief Set the On Message OnClick object
             * 
             * @param onMessageCallback 
             */
            void setOnMessageCallback(MessageCallback &&onMessageCallback) noexcept override;

            /**
             * @brief Set log
             * @param logService instance
             */
            void setLogService(const LogService *logService) noexcept override;

            /**
             * @brief Initialize lib
             */
            void initialize() override;

            /**
             * @brief Start service
             * @exception runtime_error when hardware requisites mismatch
             */
            void start() override;

            /**
             * Publish package
             * @param package pointer
             * @exception runtime_error when hardware requisites mismatch
             */
            void publish(const Head::Ptr &package) override;

            /**
             * Publish package
             * @param package pointer data
             * @param size length of msg
             * @exception runtime_error when hardware requisites mismatch
             */
            void publish(const uint8_t *package, int size) override;
        };
    }
}