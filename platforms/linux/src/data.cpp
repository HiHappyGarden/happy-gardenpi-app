/***************************************************************************
 *
 * Hi Happy Garden Interfaces
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "config.h"
#include "hhg-platform/data.hpp"
#include "osal/osal.hpp"
#include "errors.hpp"
#include "errno.h"

#include <stdlib.h>
#include <stdio.h>

namespace hhg::platform
{
inline namespace v1
{

bool data::exist_data() const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_DATA, "r");
    if(file)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool data::exist_conf() const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_CONF, "r");
    if(file)
    {
        fclose(file);
        return true;
    }
    return false;
}

bool data::read_data(string<FILE_SIZE>& data_json, class error **error) const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_DATA, "r");
    if(file == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::NO_READ), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }


    char buffer[5];
    size_t read = fread(buffer, 1, sizeof(buffer) - 1 , file);
    while(read == sizeof(buffer) - 1)
    {
        data_json += buffer;
    }
    if(read)
    {
        buffer[read + 1] = '\0';
        data_json += buffer;
    }

    fclose(file);
    return true;
}

bool data::read_conf(string<FILE_SIZE>& data_conf, class error **error) const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_DATA, "r");
    if(file == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::NO_READ), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }

    char buffer[5];
    size_t read = fread(buffer, 1, sizeof(buffer) - 1 , file);
    while(read == sizeof(buffer) - 1)
    {
        data_conf += buffer;
    }
    if(read)
    {
        buffer[read + 1] = '\0';
        data_conf += buffer;
    }


    fclose(file);
    return true;
}


bool data::write_data(const string<FILE_SIZE>& data_json, class error **error) const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_DATA, "w");
    if(file == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }

    fwrite(data_json.c_str(), 1, data_json.length(), file);

    fclose(file);
    return true;
}

bool data::write_conf(const string<FILE_SIZE>& data_conf, class error **error) const OS_NOEXCEPT
{
    FILE* file = fopen(HHGARDEN_FILE_PATH_DATA, "w");
    if(file == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }

    fwrite(data_conf.c_str(), 1, data_conf.length(), file);

    fclose(file);
    return true;
}


}
}
