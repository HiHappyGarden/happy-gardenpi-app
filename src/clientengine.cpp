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

namespace hgardenpi
{
    inline namespace v1
    {
        static const LogService *logService = nullptr;
        static string serial;

        void ClientEngine::setLogService(const LogService *logService) noexcept
        {
            if (hgardenpi::v1::logService == nullptr)
            {
                hgardenpi::v1::logService = logService;
            }
        }

        inline void ClientEngine::setInfos(const string &serial) noexcept
        {
            if (hgardenpi::v1::serial.empty())
            {
                hgardenpi::v1::serial = serial;
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

            //decode message
            auto head = protocol::decode(data);

            //if it's a syn pkg then check if it's a first connection
            if (head->flags | SYN)
            {
                //upcast to syn
                if (auto *ptr = dynamic_cast<Synchro *>(head->deserialize()))
                {

                    //check if client it's already connected
                    ClientsConnected::iterator it = clientsConnected.find(head->id);
                    if(it == clientsConnected.end())
                    {
                        //first connection

                        clientsConnected[head->id] = ClientConnected{
                                .id = head->id,
                                .syn = Synchro::Ptr(ptr),
                                .last = Synchro::Ptr(ptr),
                                .lastConnection = time(0)
                        };

                        Synchro back;
                        back.setSerial(serial);

                        auto dataToSend = encode(&back, ACK);

                        sendBackData(clientsConnected[head->id], dataToSend[0]);
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
