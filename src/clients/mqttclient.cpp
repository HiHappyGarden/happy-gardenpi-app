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

#include "mqttclient.hpp"

#include <cstring>

#include "../services/logservice.hpp"

#include "../constants.hpp"

namespace hgardenpi
{
    inline namespace v1
    {

        MQTTClient::MQTTClient(const string &serial, const string &host, const string &user, const string &passwd, uint16_t port, uint16_t keepAlive) : topic("/HappyGardenPI/" + serial),
                                                                                                                                                        user(user),
                                                                                                                                                        passwd(passwd)
        {
            connect(host.c_str(), port, keepAlive);
        }

        void MQTTClient::on_connect(int rc)
        {

            cout << "---1: " << endl;
            if (rc)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW(strerror(rc))
            }
            cout << "---2: " << endl;
            rc = username_pw_set(user.c_str(), passwd.c_str());
            if (rc)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW(strerror(rc))
            }
            cout << "---3: " << endl;
            rc = subscribe(nullptr, topic.c_str());
            if (rc)
            {
                HGARDENPI_ERROR_LOG_AMD_THROW(strerror(rc));
            }
            cout << "---4: " << endl;
            LogService::getInstance()
                ->write(LOG_INFO, "%s %s", "submitted topic", topic.c_str());
        }

        void MQTTClient::on_message(const struct mosquitto_message *message)
        {
            int payloadSize = message->payloadlen + 1;
            char *buf = new (nothrow) char[payloadSize];
            if (buf)
            {
                memset(buf, 0, payloadSize * sizeof(char));
                memcpy(buf, message->payload, payloadSize * sizeof(char));
                cout << "message recieved: "
                     << buf << endl;
                delete[] buf;
            }
        }

    }

}
