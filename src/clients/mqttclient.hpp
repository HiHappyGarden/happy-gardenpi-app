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

#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        using mosqpp::mosquittopp;
        using std::move;
        using std::string;

        /**
         * @brief MQTT Client for connect to mosquitto
         */
        class MQTTClient final : public mosquittopp
        {

            static inline const constexpr uint16_t KEEP_ALIVE = 60;
            static inline const constexpr uint16_t PORT = 60;

        public:
            HGARDENPI_NO_COPY_NO_MOVE(MQTTClient)

            /**
             * @brief Construct a new MQTTClient object
             * 
             * @param id client name 
             * @param host server host
             * @param port server port
             */
            MQTTClient(const string &id, const string &host, uint16_t port = MQTTClient::PORT);

            /**
             * @brief Construct a new MQTTClient object
             * 
             * @param id client name 
             * @param host server host
             * @param port server port
             */
            inline MQTTClient(const string &&id, const string &&host, uint16_t port = MQTTClient::PORT) : MQTTClient(id, host, port)
            {
            }
        };
    }
}