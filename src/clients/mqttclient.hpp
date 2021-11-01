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

#include <string>
#include <functional>
#include <hgardenpi-protocol/head.hpp>

#include "../interfaces/loggable.hpp"
#include "../interfaces/startable.hpp"
#include "../interfaces/initializable.hpp"



namespace hgardenpi
{
    inline namespace v1
    {
        using namespace std;
        using namespace protocol;

        /**
         * @brief Generic MQTT Client for connect to mosquitto
         * 
         * @tparam T callback type 
         */
        class MQTTClient : public Loggable, public Initializable, public Startable
        {

        public:
            /**
             * @brief Callback on message received
             */
            using MessageCallback = function<void(const uint8_t *, int)>;

            MQTTClient() = default;
            virtual ~MQTTClient() = default;
            HGARDENPI_NO_COPY_NO_MOVE(MQTTClient)

            /**
             * @brief start loop
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void loop() = 0;

            /**
             * @brief Set the On Message OnClick object
             * 
             * @param onMessageCallback 
             */
            virtual void setOnMessageCallback(MessageCallback &&onMessageCallback) = 0;

            /**
             * Publish package
             * @param head pointer package
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void publish(const Head::Ptr &head) = 0;

            /**
             * Publish package
             * @param package Pointer data
             * @param size length of msg
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void publish(const uint8_t *package, int size) = 0;

            /**
             * Publish package
             * @param package Buffer data
             * @param size length of msg
             * @exception runtime_error when hardware requisites mismatch
             */
            virtual void publish(const Buffer &) = 0;
        };
    }

}