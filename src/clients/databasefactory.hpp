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

#include "sqlite3database.hpp"
#include "../services/logservice.hpp"

namespace hgardenpi
{

    inline namespace v1
    {

/**
 * Questa classe serve da collettore per scegliere quale db usare, adessso è implementato solo sqlite
 */
        class DatabaseFactory
        {
        public:
            enum class Type
            {
                SQLITE3
            };

        private:
            Type type = Type::SQLITE3;

            Database::UpdateDBVersion updateDBVersion = nullptr;
            LogService *logWriter = nullptr;

            const char **createDB;
        public:
            Database *getInstance(const string &);

            Database *getInstance(string &&);

            inline DatabaseFactory &setType(const Type &type)
            {
                this->type = type;
                return *this;
            }

            inline DatabaseFactory &setType(Type &&type)
            {
                this->type = type;
                return *this;
            }

            inline DatabaseFactory &setUpdateDBVersion(Database::UpdateDBVersion updateDBVersion)
            {
                this->updateDBVersion = move(updateDBVersion);
                return *this;
            }

            inline DatabaseFactory &setLogWriter(LogWriter logWriter)
            {
                this->logWriter = move(logWriter);
                return *this;
            }

            inline DatabaseFactory &setCreateDB(const char *createDB[])
            {
                this->createDB = createDB;
                return *this;
            }
        };
    }
}


