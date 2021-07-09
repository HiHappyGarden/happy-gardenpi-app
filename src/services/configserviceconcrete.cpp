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

#include "configserviceconcrete.hpp"

#include <fstream>
#include <iostream>
#include <fstream>
using namespace std;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define HGARDENPI_READ_STRING(conatiner, field, value)                                                                \
    if (auto &&containerObj = jsonConfig[conatiner]; containerObj.contains(value) && containerObj[value].is_string()) \
    {                                                                                                                 \
        ret->field = containerObj[value].get<string>();                                                               \
    }

#define HGARDENPI_READ_UINT16(conatiner, field, value)                                                                \
    if (auto &&containerObj = jsonConfig[conatiner]; containerObj.contains(value) && containerObj[value].is_string()) \
    {                                                                                                                 \
        ret->field = containerObj[value].get<uint16_t>();                                                             \
    }

namespace hgardenpi
{
    inline namespace v1
    {

        ConfigInfo::Ptr ConfigSerivceConcrete::read() noexcept
        {
            ConfigInfo::Ptr ret = make_shared<ConfigInfo>();

            ifstream config(configFile);
            if (config.is_open())
            {
                string buf;
                string line;
                while (getline(config, line))
                {
                    buf += line;
                }
                config.close();

                auto &&jsonConfig = json::parse(buf);

                if (jsonConfig.contains("system") && jsonConfig.is_object())
                {
                    if (auto &&containerObj = jsonConfig["system"]; containerObj.contains("fileLock") && containerObj["fileLock"].is_string())
                    {
                        ret->fileLock = containerObj["fileLock"].get<string>();
                    }
                }
                if (jsonConfig.contains("broker") && jsonConfig.is_object())
                {
                    auto &&containerObj = jsonConfig["broker"];
                    if (containerObj.contains("host") && containerObj["host"].is_string())
                    {
                        ret->broker.host = containerObj["host"].get<string>();
                    }
                    if (containerObj.contains("port") && containerObj["port"].is_string())
                    {
                        ret->broker.port = containerObj["port"].get<int>();
                    }
                    if (containerObj.contains("user") && containerObj["user"].is_string())
                    {
                        ret->broker.user = containerObj["user"].get<string>();
                    }
                    if (containerObj.contains("passwd") && containerObj["passwd"].is_string())
                    {
                        ret->broker.passwd = containerObj["passwd"].get<string>();
                    }
                }
            }

            cout << ret->toString() << endl;
            return ret;
        }

    }
}