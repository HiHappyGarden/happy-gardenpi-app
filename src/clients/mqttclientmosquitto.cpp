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

#include "mqttclientmosquitto.hpp"

#include <mosquitto.h>
#include <unistd.h>

#include <sstream>

#include "../threadengine.hpp"
#include "../config.h"

namespace hgardenpi
{
    inline namespace v1
    {
        //todo: place here in global
        static hgardenpi::v1::MQTTClient::MessageCallback onMessageCallback;

        static LogService *logService = nullptr;

        MQTTClientMosquitto::MQTTClientMosquitto(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port, uint16_t keepAlive)
            : topic(move("/HappyGardenPI/" + serial)),
              host(host),
              port(port),
              keepAlive(keepAlive),
              user(user),
              passwd(passwd)
        {
            /*
            * new instance client
            */
            ostringstream ss;
            ss << HGARDENPI_NAME;
            ss << "_";
            ss << pidMain;

            mosq = mosquitto_new(ss.str().c_str(), true, nullptr);
            if (!mosq)
            {
                throw runtime_error("no memory available");
            }
        }

        MQTTClientMosquitto::~MQTTClientMosquitto() noexcept
        {
            if (mosq)
            {
                mosquitto_destroy(mosq);
            }
            if (initalizated)
            {
                mosquitto_lib_cleanup();
            }
        }

        void MQTTClientMosquitto::start()
        {
            if (!initalizated)
            {
                throw runtime_error("MQTTClientMosquitto not initialized");
            }

            //check il moquitto client is allocated
            if (mosq)
            {
                /*
                * Connection callback
                * call when the broker send CONNACK
                */
                mosquitto_connect_callback_set(mosq, [](mosquitto *mosq, void *obj, int result)
                {
                    if (result != MOSQ_ERR_SUCCESS)
                    {
                        string err("mosquitto client connection error: ");
                        err.append(mosquitto_strerror(result));
                        cerr << err << endl;

                        //keep here macro HGARDENPI_ERROR_LOG_AMD_THROW
                        HGARDENPI_ERROR_LOG_AMD_THROW(err.c_str())
                    }
                });

                /*
                * Subscription callback
                * call when the message will send successfully to broker
                */
                mosquitto_message_callback_set(mosq, [](mosquitto *, void *, const mosquitto_message *message)
                {
                    if (message->payloadlen)
                    {
                        hgardenpi::v1::onMessageCallback((uint8_t *)message->payload, message->payloadlen);
                    }
                    else
                    {
                        hgardenpi::v1::onMessageCallback(nullptr, 0);
                    }

                });

                //set username and passwd
                logService->write(LOG_INFO, "broker %s: %s", "user", user.c_str());
                int rc = mosquitto_username_pw_set(mosq, user.c_str(), passwd.c_str());
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    string err("set user and password: ");
                    err.append(mosquitto_strerror(rc));
                    throw runtime_error(err.c_str());
                }

                //set protocol version
                int ver = MQTT_PROTOCOL_V311;
                mosquitto_opts_set(mosq, MOSQ_OPT_PROTOCOL_VERSION, &ver);

                //connect
                logService->write(LOG_INFO, "broker %s: %s:%d", "host", host.c_str(), port);
                if (mosquitto_connect(mosq, host.c_str(), port, keepAlive) != MOSQ_ERR_SUCCESS)
                {
                    string err("connection: ");
                    err.append(mosquitto_strerror(rc));
                    throw runtime_error(err.c_str());
                }

                logService->write(LOG_INFO, "broker %s: %s", "topic", topic.c_str());
                mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
            }
        }

        void MQTTClientMosquitto::loop()
        {

            while(mosquitto_loop(mosq, 0, 1) == MOSQ_ERR_SUCCESS && wiringPiRunningThread)
            {
                threadSleep(Time::TICK);
            }
        }

        inline void MQTTClientMosquitto::setOnMessageCallback(MQTTClient::MessageCallback &&onMessageCallback) noexcept
        {
            hgardenpi::v1::onMessageCallback = move(onMessageCallback);
        }

        inline void MQTTClientMosquitto::setLogService(const LogService *logService) noexcept
        {
            hgardenpi::v1::logService = const_cast<LogService *>(logService);
        }

        void MQTTClientMosquitto::initialize()
        {
            //initialize mosquittopp
            if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
            {
                throw runtime_error("mosquitto_lib_init() error");
            }
            initalizated = true;
        }


    }

}
