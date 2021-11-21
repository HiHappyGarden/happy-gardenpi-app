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
// Created by Antonio Salsi on 01/11/21.
//

#include "clientengine.hpp"

#include <syslog.h>
#include <iostream>
using namespace std;

#include "services/logservice.hpp"
using namespace hgardenpi::protocol;

#include "clients/mqttclientmosquitto.hpp"

#include <hgardenpi-protocol/utilities/stringutils.hpp>
using hgardenpi::protocol::stringHexToString;

namespace hgardenpi
{
    inline namespace v1
    {
        static const LogService *logService = nullptr;
        static string serial;
        static ConfigInfo::Ptr info = nullptr;

        void ClientEngine::setLogService(const LogService *logService) noexcept
        {
            if (hgardenpi::v1::logService == nullptr)
            {
                hgardenpi::v1::logService = logService;
            }
        }

        void ClientEngine::setInfos(const string &serial, const ConfigInfo::Ptr &info) noexcept
        {
            if (hgardenpi::serial.empty())
            {
                hgardenpi::serial = serial;
                hgardenpi::info = info;
            }
        }

        void ClientEngine::operator()(const uint8_t *data, int len) noexcept try
        {
            //check if message is not null
            if (len == 0 || data == nullptr)
            {
                logService->write(LOG_WARNING, "wrong message length 0");
                return;
            }

            cout << stringHexToString(data, len) << endl;

            //decode message
            auto head = protocol::decode(data);

            //if it's a syn pkg then check if it's a first connection
            if (head->flags | SYN)
            {
                //upcast to syn
                if (auto *ptr = dynamic_cast<Synchro *>(head->deserialize()))
                {

                    auto &&serial = ptr->getSerial();
                    if (serial.empty())
                    {
                        logService->write(LOG_WARNING, "unable start connection with client, serial not found");
                        //TODO: error response
                        return;
                    }

                    //check if client it's already connected
                    ClientsConnected::iterator it = clientsConnected.find(head->id);
                    if(it == clientsConnected.end())
                    {
                        //first connection

                        //create new instance off client reference
                        clientsConnected[head->id] = ClientConnected {
                                .id = head->id,
                                .syn = Synchro::Ptr(ptr),
                                .last = Synchro::Ptr(ptr),
                                .lastConnection = time(0),
                                .mqttClientTX = new(nothrow) MQTTClientMosquitto(serial,
                                    info->broker.host,
                                    info->broker.user,
                                    info->broker.passwd,
                                    info->broker.port)
                        };
                        auto mqttClientTX = clientsConnected[head->id].mqttClientTX;
                        if (!mqttClientTX)
                        {
                            throw runtime_error("no memory for mqttClientTX");
                        }

                        mqttClientTX->initialize();
                        mqttClientTX->start();

                        string msg("start communication with client: ");
                        msg += serial;
                        logService->write(LOG_INFO, msg.c_str());

                        //respond to client
                        Synchro back;
                        back.setSerial(hgardenpi::serial);

                        auto dataToSend = encode(&back, ACK);;

                        logService->write(LOG_INFO, "publish topic: %s msg: %s"
                                          , mqttClientTX->getTopic().c_str()
                                          , stringHexToString(dataToSend[0].first.get(), dataToSend[0].second).c_str()
                                          );
                        mqttClientTX->publish(dataToSend[0]);

                    }

                    cout << ptr->getSerial() << endl;

                    delete ptr;
                }
            }
        }
        catch (const runtime_error &e)
        {
            cerr << e.what() << endl;
        }
        catch (...)
        {
            logService->write(LOG_ERR, "ClientEngine generic error");
        }

    }
}
