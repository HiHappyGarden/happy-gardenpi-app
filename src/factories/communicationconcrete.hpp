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

#pragma once

#include "communication.hpp"
#include "../clients/mqttclient.hpp"
#include "../clientengine.hpp"

#include <map>

namespace hgardenpi
{

    inline namespace v1
    {

        /**
         * @brief Concrete implementation of communication factory
         */
        class CommunicationConcrete final : public Communication
        {

            string serial;
            ConfigInfo::Ptr info;

            LogService *logService;
            MQTTClient *mqttClientRX = nullptr;
            ClientEngine::ClientsConnected clientsConnected;
            ClientEngine clientEngine;

        public:
            CommunicationConcrete() noexcept;
            inline ~CommunicationConcrete() noexcept override
            {
                if (mqttClientRX)
                {
                    delete mqttClientRX;
                    mqttClientRX = nullptr;
                }
                for (auto &&[key, value] : clientsConnected)
                {
                    if(value.mqttClientTX)
                    {
                        delete value.mqttClientTX;
                    }
                }
            }

            /**
             * @brief Before all call this function fo initialize the project
             *
             * @exception runtime_error when hardware requisites mismatch
             */
            void initialize() override;

            /**
             * @brief Start main look and scheduler
             * @exception runtime_error when hardware requisites mismatch
             */
            void start() override;

//            /**
//             * @brief Get the Mqtt Client Rx object
//             *
//             * @return const MQTTClient* mqtt client instance
//             */
//            [[nodiscard]] inline MQTTClient *getMqttRx() const noexcept override
//            {
//                return mqttRx;
//            }

            /**
             * Set infos for initialize components
             * @param serial HW serial
             * @param info Configuration info
             */
            inline void setInfos(const string &serial, const ConfigInfo::Ptr &info) noexcept override
            {
                this->serial = serial;
                this->info = info;
            }

            /**
             * @brief Set log
             * @param logService instance
             */
            void setLogService(const LogService *logService) noexcept override;

            /**
             * @brief Enable loop
             */
            inline void loop() noexcept override
            {
                mqttClientRX->loop();
            }

            /**
             * @brief Return the name of object
             *
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }
        };
    }
}


