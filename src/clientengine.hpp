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

#pragma once

#include <hgardenpi-protocol/protocol.hpp>
#include <hgardenpi-protocol/packages/synchro.hpp>
#include <map>
#include <functional>

namespace hgardenpi
{
    inline namespace v1
    {

        using namespace hgardenpi::protocol;
        using std::map;

        class LogService;
        class MQTTClient;

        /**
         * @brief Class for managing connection between server and clients
         * @note for this class not need solid paradigm
         */
        class ClientEngine final
        {

            /**
             * @brief Facility class for manage client connection
             */
            struct ClientConnected final //TODO: maybe move on communication.hpp
            {

                uint8_t id = 0;
                /**
                 * @brief first package for starting communication
                 */
                Synchro::Ptr syn = nullptr;
                /**
                 * @brief last package for starting communication
                 */
                Package::Ptr last = nullptr;
                /**
                 * @brief communication client instance
                 */
                MQTTClient *clientCommunication = nullptr;
                /**
                 * @brief time to last package send
                 */
                time_t lastConnection = 0;
                /**
                 * @brief update lastConnection time
                 */
                inline void updateLastConnection() noexcept {
                    lastConnection = time(0);
                }
            };
        public:
            /**
             * @brief type for managing clients connections
             */
            typedef map<uint8_t, ClientConnected> ClientsConnected; //TODO: maybe move on communication.hpp

            /**
             * @brief type to define responses to client
             */
            using SendBackData = std::function<void (ClientConnected &, const Buffer &)>;
        private:
            ClientsConnected &clientsConnected;
            SendBackData sendBackData;
        public:
            explicit inline ClientEngine(ClientsConnected &clientsConnected, const SendBackData &sendBackData)
            : clientsConnected(clientsConnected)
            , sendBackData(sendBackData)
            {}

            /**
             * @brief Set log
             * @param logService instance
             */
            void setLogService(const LogService *logService) noexcept;

            /**
             * Set infos for initialize components
             * @param serial hw serial
             */
            void setInfos(const string &serial) noexcept;

            /**
             * @brief functor for callback when any data in from client
             * @param data send from client
             * @param len length of data
             */
            void operator() (const uint8_t *data, int len) noexcept;
        };

    }
}

