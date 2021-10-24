// MIT License
//
// Copyright (c) 2021. Happy GardenPI
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
//

//
// Created by Antonio Salsi on 24/08/21.
//

#include "communicationconcrete.hpp"

#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/utilities/stringutils.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>
#include <iostream>
using namespace std;

#include "../clients/mqttclientmosquitto.hpp"
#include "../services/logservice.hpp"



using namespace hgardenpi;

static CommunicationConcrete *self;
static LogService *logService;

/**
* @brief Event triggered when MqttClient message arrive
* @param data message data
*/
static void onMqttClientMessageCallback(const uint8_t *data, int len);

void CommunicationConcrete::initialize()
{
    self = this;
    mqttRx = new (nothrow) MQTTClientMosquitto(serial,
                                               info->broker.host,
                                               info->broker.user,
                                               info->broker.passwd,
                                               info->broker.port
    );
    if(!mqttRx)
    {
        throw runtime_error("no memory for mqttRx");
    }
    int major = 0, minor = 0, patch = 0;
    protocol::getVersion(major, minor, patch);

    logService->write(LOG_INFO, "happy-gardenpi-protocol version: %d.%d.%d", major, minor, patch);

    mqttRx->setLogService(logService);

    mqttRx->setOnMessageCallback(&onMqttClientMessageCallback);

    mqttRx->initialize();
}

void CommunicationConcrete::start()
{
    mqttRx->start();
}

inline void CommunicationConcrete::setLogService(const LogService *logService) noexcept
{
    ::logService = const_cast<LogService *>(logService);
}


void onMqttClientMessageCallback(const uint8_t *data, int len)
{

    if (len == 0 || data == nullptr)
    {
        logService->write(LOG_WARNING,"wrong message length 0");
        return;
    }

    cout << stringHexToString(data, len) << endl;
    try
    {
        auto head = protocol::decode(data);

        if (auto *ptr = dynamic_cast<Synchro *>(head->deserialize()))
        {
            cout << ptr->getSerial() << endl;

            delete ptr;
        }

        //const uint8_t *s = reinterpret_cast<uint8_t *>(package->payload);

        //cout << s << endl;
    }
    catch (const runtime_error &e)
    {
        cerr << e.what() << endl;
    }


}