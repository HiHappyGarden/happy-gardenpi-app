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

//todo: place here in global
static hgardenpi::v1::MQTTClient::MessageCallback onMessageCallback;

namespace hgardenpi
{
    inline namespace v1
    {

        MQTTClientMosquitto::MQTTClientMosquitto(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port, uint16_t keepAlive)
            : topic("/HappyGardenPI/" + serial),
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
            ss << pidMain;

            mosq = mosquitto_new(ss.str().c_str(), true, nullptr);
            if (!mosq)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("no memory available")
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
                throw runtime_error(_("MQTTClientMosquitto not initialized"));
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
                        string err("connection error: ");
                        err.append(mosquitto_strerror(result));
                        cerr << err << endl;
                    }
                });

                /*
                * Subscription callback
                * call when the message will send successfully to broker
                */
                mosquitto_message_callback_set(mosq, [](mosquitto *mosq, void *obj, const mosquitto_message *message)
                {
//                    bool match = 0;
//                    printf("received message '%.*s' for topic '%s'\n", message->payloadlen, (char *)message->payload, message->topic);

                    ::onMessageCallback((uint8_t *)message->payload);

                    //todo
                    //    mosquitto_topic_matches_sub(, message->topic, &match);
                    //    if (match)
                    //    {
                    //        printf("received message for Telemetry topic\n");
                    //    }
                });

                //set username and passwd
                logService->write(LOG_INFO, "broker %s: %s", "user", user.c_str());
                int rc = mosquitto_username_pw_set(mosq, user.c_str(), passwd.c_str());
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    string err("set user and password: ");
                    err.append(mosquitto_strerror(rc));
                    logService->write(LOG_ERR, "%s", err.c_str());
                    throw runtime_error(err);
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
                    logService->write(LOG_ERR, "%s", err.c_str());
                    throw runtime_error(err);
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

//            int rc = mosquitto_loop_forever(mosq, static_cast<int>(Time::TICK), 1);
//            if (!wiringPiRunningThread && rc)
//            {
//                logService->write(LOG_WARNING, "%s", "connection error! Try to reconnect");
//                threadSleep(5'000);
//                rc = mosquitto_reconnect(mosq);
//                if (rc != MOSQ_ERR_SUCCESS)
//                {
//                    HGARDENPI_ERROR_LOG_AMD_THROW("reconnection fail")
//                }
//            }
        }

        void MQTTClientMosquitto::initialize()
        {
            //initialize mosquittopp
            if (mosquitto_lib_init() != MOSQ_ERR_SUCCESS)
            {
                string msg("mosquitto_lib_init() error");
                logService->write(LOG_ERR, "%d", msg.c_str());
                throw runtime_error(_(msg.c_str()));
            }
            initalizated = true;
        }

        void MQTTClientMosquitto::setOnMessageCallback(MQTTClient::MessageCallback &&onMessageCallback) noexcept
        {
            ::onMessageCallback = onMessageCallback;
        }


    }

}
