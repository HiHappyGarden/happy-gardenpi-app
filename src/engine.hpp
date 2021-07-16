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

#pragma once

#include <memory>
#include <SQLiteCpp/Database.h>

#include "interfaces/singleton.hpp"
#include "factories/factory.hpp"
#include "factories/factoryconcrete.hpp"
#include "clients/mqttclient.hpp"

namespace hgardenpi
{
    inline namespace v1
    {
        using SQLite::Database;

        /**
         * @brief Singleton where are put global variable
         */
        class Engine final : public Singleton<Engine>
        {

            friend void initialize();
            friend void start();

            Factory *factory = nullptr;
            MQTTClient *mqttClient = nullptr;
            Database *database = nullptr;
        public:
            Engine() = default;
            ~Engine() noexcept override;
            HGARDENPI_NO_COPY_NO_MOVE(Engine)

            /**
             * @brief Return the name of object
             * 
             * @return std::string name of object
             */
            inline string toString() noexcept override
            {
                return typeid(*this).name();
            }

            /**
             * @brief Set the Mqtt Client object
             * 
             * @param mqttClient mqtt client instance
             */
            inline void settMqttClient(const MQTTClient *mqttClient) noexcept
            {
                this->mqttClient = const_cast<MQTTClient *>(mqttClient);
            }

            /**
             * @brief Get the Mqtt Client object
             * 
             * @return const MQTTClient* mqtt client instance
             */
            inline const MQTTClient *getMqttClient() const noexcept
            {
                return mqttClient;
            }

            /**
             * @brief Set the Database object
             *
             * @param database database instabce
             */
            inline void settDatabase(const Database *database) noexcept
            {
                this->database = const_cast<Database *>(database);
            }

            /**
             * @brief Get the Mqtt Client object
             *
             * @return const MQTTClient* mqtt client instance
             */
            inline const Database *getDatabase() const noexcept
            {
                return database;
            }
        };

        /**
         * @brief Before all call this functiuon fo inilialize the project
         * 
         * @exception runtime_error when hardware requisites mismatch
         */
        void initialize();

        /**
         * @brief Start main look and scheduler
         * 
         */
        void start();

    }
}