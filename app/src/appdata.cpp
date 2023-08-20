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

#include "hhg-app/appdata.hpp"
#include "hhg-app/appjson.hpp"
#include "hhg-intf/data.hpp"
#include "errors.hpp"

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "APP DATA";

}

bool app_data::load(error **error) OS_NOEXCEPT
{
    {
        if(!hardware.get_data()->exist_conf())
        {
            OS_LOG_WARNING(APP_TAG, "No conf to load write default");
            if(!save_conf(error))
            {
                *error = OS_ERROR_BUILD(*error, "save() fail", static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                return false;
            }
            return true;
        }

        os::string<intf::data::FILE_SIZE>data;
        if(!hardware.get_data()->read_conf(data, error))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::NO_READ), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return false;
        }

        string<intf::data::FILE_SIZE> json;
        if(!parse(json, schedules, error))
        {
            *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            return false;
        }
    }

    if(conf.header.marker != 0xC0)
    {
        {
            *error = OS_ERROR_BUILD(*error, "wrong marker", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            return false;
        }
    }

    switch (conf.header.version)
    {
    case 1:

        break;
    default:
        break;
    }


    {
        if(!hardware.get_data()->exist_data())
        {
            OS_LOG_WARNING(APP_TAG, "No data to load write default");
            if(!save_data(error))
            {
                *error = OS_ERROR_BUILD(*error, "save() fail", static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                return false;
            }
            return true;
        }

        os::string<intf::data::FILE_SIZE>data;
        if(!hardware.get_data()->read_data(data, error))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::NO_READ), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return false;
        }

        string<intf::data::FILE_SIZE> json;
        if(!parse(json, schedules, error))
        {
            *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            return false;
        }
    }

    return false;
}

bool app_data::save_conf(error **error) const OS_NOEXCEPT
{

    string<intf::data::FILE_SIZE>&& json = print(conf, error);
    if(error && *error)
    {
        *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::JSON_PRINT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        return false;
    }

    if(!hardware.get_data()->write_conf(json, error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    return true;
}

bool app_data::save_data(error **error) const OS_NOEXCEPT
{

    string<intf::data::FILE_SIZE>&& json = print(schedules, error);
    if(error && *error)
    {
        *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::JSON_PRINT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        return false;
    }

    if(!hardware.get_data()->write_data(json, error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(*error, "read_data() fail", static_cast<uint8_t>(error_code::NO_WRITE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    return true;
}

}
}

