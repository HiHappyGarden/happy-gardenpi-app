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

#include <string>
#include <memory>
#include <functional>

namespace hgardenpi
{
    inline namespace v1
    {
        using namespace std;

        /**
         * @brief Generic MQTT Client for connect to mosquitto
         * 
         * @tparam T callback type 
         */
        class MQTTClient
        {

        public:
            using Ptr = shared_ptr<MQTTClient>;
            using MessageCallback = function<void(uint8_t *)>;

            MQTTClient() = default;
            virtual ~MQTTClient() = default;

            /**
             * @brief start loop 
             * 
             * @param run if the loop is in execution
             */
            virtual void loop(volatile bool &run) = 0;

            /**
             * @brief Set the On Message Callback object
             * 
             * @param onMessageCallback 
             */
            virtual void setOnMessageCallback(MessageCallback &&onMessageCallback) = 0;
        };
    }

}