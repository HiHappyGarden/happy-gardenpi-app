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

#include <string>

#include "../interfaces/object.hpp"
#include "../interfaces/initializable.hpp"
#include "../interfaces/startable.hpp"
#include "../interfaces/loopable.hpp"
#include "../pods/configinfo.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

        class MQTTClient;
        class LogService;

        /**
         * @brief Abstract factory for communication system
         *
         */
        class Communication : public Object, public Initializable, public Startable, public Loopable
        {
        public:
            Communication() = default;

            virtual ~Communication() = default;
            HGARDENPI_NO_COPY_NO_MOVE(Communication)

            /**
             * Set infos for initialize components
             * @param serial hw serial
             * @param info configutaion info
             */
            virtual void setInfos(const std::string &serial, const ConfigInfo::Ptr &info) noexcept = 0;

            /**
             * @brief Set the Log Service object
             * @param logService instance
             */
            virtual void setLogService(const LogService *) noexcept = 0;

            /**
             * @brief Get the Mqtt Client Rx object
             * @return const MQTTClient* mqtt client instance
             */
//            virtual MQTTClient *getMqttRx() const noexcept = 0;

        };

    }
}
