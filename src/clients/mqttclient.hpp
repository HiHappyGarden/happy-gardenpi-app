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

#include <mosquittopp.h>

#include <string>
#include <memory>
#include <functional>
#include <iostream>

#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        using mosqpp::mosquittopp;
        using namespace std;

        /**
         * @brief MQTT Client for connect to mosquitto
         */
        class MQTTClient final : public mosquittopp
        {

            static inline const constexpr uint16_t KEEP_ALIVE = 60;
            static inline const constexpr uint16_t PORT = 1883;

            const string topic;
            const string &user;
            const string &passwd;

        public:
            using MessageCallback = function<void(const struct mosquitto_message *)>;
            typedef shared_ptr<MQTTClient> Ptr;

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
            MQTTClient(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port = MQTTClient::PORT, uint16_t keepAlive = MQTTClient::KEEP_ALIVE);

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
            inline MQTTClient(const string &serial, const string &&host, const string &&user, const string &&passwd, uint16_t port = MQTTClient::PORT, uint16_t keepAlive = MQTTClient::KEEP_ALIVE) : MQTTClient(serial, host, user, passwd, port, keepAlive)
            {
            }

            HGARDENPI_NO_COPY_NO_MOVE(MQTTClient)

            /**
             * @brief Set the On Message Callback object
             * 
             * @param onMessageCallback 
             */
            inline void setOnMessageCallback(MessageCallback &&onMessageCallback) noexcept
            {
                this->onMessageCallback = onMessageCallback;
            }

        private:
            MessageCallback onMessageCallback;

            void on_connect(int rc) override;

            void on_message(const struct mosquitto_message *message) override;

            inline void on_subscribe(int mid, int qos_count, const int *granted_qos) override
            {
                cout << to_string(mid) << "-" << to_string(qos_count) << endl;
            }

            inline void on_log(int level, const char *str) override
            {
                cout << to_string(level) << "-" << str << endl;
            }
        };
    }
}