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


#include "mqttclientlocalsub.hpp"

#include <mosquitto.h>
#include <unistd.h>

#include <cstring>
#include <sstream>

#include "../services/logservice.hpp"
#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        MQTTClientLocalSub::MQTTClientLocalSub(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port, uint16_t keepAlive)
            : topic("/HappyGardenPI/" + serial),
              user(user),
              passwd(passwd)
        {

            ostringstream ss;
            ss << getpid();

            /* 
            * new instance client
            */
            mosq = mosquitto_new(ss.str().c_str(), true, nullptr);
            if (mosq)
            {
                /* 
                * Connection callback
                * call when yhe broker send CONNACK
                */
                mosquitto_connect_callback_set(mosq, [](mosquitto *mosq, void *obj, int result)
                                               {
                                                   if (result != MOSQ_ERR_SUCCESS)
                                                   {
                                                       string err("connection error: ");
                                                       err.append(mosquitto_strerror(result));
                                                       LogService::getInstance()->write(LOG_ERR, "%s", err.c_str());
                                                       cerr << err << endl;
                                                   }
                                               });

                /* 
                * Subscription callback
                * call when the message will senf succesfully to broker
                */
                mosquitto_message_callback_set(mosq, [](mosquitto *mosq, void *obj, const mosquitto_message *message)
                                               {
                                                   bool match = 0;
                                                   printf("received message '%.*s' for topic '%s'\n", message->payloadlen, (char *)message->payload, message->topic);

                                                   //todo
                                                   //    mosquitto_topic_matches_sub(, message->topic, &match);
                                                   //    if (match)
                                                   //    {
                                                   //        printf("received message for Telemetry topic\n");
                                                   //    }
                                               });

                int rc = mosquitto_username_pw_set(mosq, user.c_str(), passwd.c_str());
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    string err("set user and password: ");
                    err.append(mosquitto_strerror(rc));
                    LogService::getInstance()->write(LOG_ERR, "%s", err.c_str());
                    cerr << err << endl;
                    throw runtime_error(err);
                }

                int ver = MQTT_PROTOCOL_V311;
                mosquitto_opts_set(mosq, MOSQ_OPT_PROTOCOL_VERSION, &ver);

                mosquitto_connect(mosq, host.c_str(), port, keepAlive);

                LogService::getInstance()->write(LOG_INFO, "%s: %s", "topic", topic.c_str());
                mosquitto_subscribe(mosq, nullptr, topic.c_str(), 0);
            }
            else
            {
                HGARDENPI_ERROR_LOG_AMD_THROW("no memory available")
            }
        }

        MQTTClientLocalSub::~MQTTClientLocalSub() noexcept
        {
            if (mosq)
            {
                mosquitto_destroy(mosq);
            }
        }

        void MQTTClientLocalSub::loop(volatile bool &run)
        {

            int rc = mosquitto_loop(mosq, static_cast<int>(Time::TICK), 1);

            if (run && rc)
            {
                LogService::getInstance()->write(LOG_WARNING, "%s", "connection error! Try to reconnect");
                sleep(5);
                rc = mosquitto_reconnect(mosq);
                if (rc != MOSQ_ERR_SUCCESS)
                {
                    HGARDENPI_ERROR_LOG_AMD_THROW("reconnection fail")
                }
            }
        }

    }

}
