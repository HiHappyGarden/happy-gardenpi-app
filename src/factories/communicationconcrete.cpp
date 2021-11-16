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
using namespace hgardenpi::protocol;

#include "../clients/mqttclientmosquitto.hpp"

#if HGARDENPI_TEST > 0
#include <hgardenpi-protocol/utilities/stringutils.hpp>
#endif

namespace hgardenpi
{
    inline namespace v1
    {
        CommunicationConcrete::CommunicationConcrete() noexcept
                : clientEngine(clientsConnected)
        {}

        void CommunicationConcrete::initialize()
        {
            mqttClientRX = new(nothrow) MQTTClientMosquitto(serial,
                                                            info->broker.host,
                                                            info->broker.user,
                                                            info->broker.passwd,
                                                            info->broker.port);
            if (!mqttClientRX)
            {
                throw runtime_error("no memory for mqttClientRX");
            }

            //print protocol version
            int major = 0, minor = 0, patch = 0;
            protocol::getVersion(major, minor, patch);
            logService->write(LOG_INFO, "happy-gardenpi-protocol version: %d.%d.%d", major, minor, patch);

            clientEngine.setInfos(serial, info);

            mqttClientRX->setLogService(logService);

            //management of request from client
            mqttClientRX->setOnMessageCallback(clientEngine);

            //initialize mqtt client
            mqttClientRX->initialize();
        }

        inline void CommunicationConcrete::start()
        {
            mqttClientRX->start();
        }

        inline void CommunicationConcrete::setLogService(const LogService *logService) noexcept
        {
            this->logService = const_cast<LogService *>(logService);
            clientEngine.setLogService(logService);
        }
    }
}