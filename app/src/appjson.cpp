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

#include "hhg-app/appjson.hpp"
#include "hhg-app/appdata.hpp"
#include "errors.hpp"

#include <cJSON.h>

namespace hhg::app
{
inline namespace v1
{

bool parse(const string<intf::data::FILE_SIZE> &json, class conf &conf, error **error)
{

    return true;
}

string<intf::data::FILE_SIZE> print(const class conf &conf, error **error)
{

    return {};
}


bool parse(const string<intf::data::FILE_SIZE>& json, schedule (&schedules)[HHGARDEN_SCHEDULES_SIZE], class error** error)
{

    auto root = cJSON_Parse(json.c_str());
    if (root == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    uint8_t schedule_count = 0;
    const cJSON *schedule_it = NULL;
    cJSON_ArrayForEach(schedule_it, root)
    {
        auto minute = cJSON_GetObjectItemCaseSensitive(schedule_it, "minute");
        if (cJSON_IsInvalid(minute) || !cJSON_IsNumber(minute))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].minute = minute->valueint;

        auto hour = cJSON_GetObjectItemCaseSensitive(schedule_it, "hour");
        if (cJSON_IsInvalid(hour) || !cJSON_IsNumber(hour))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].hour = hour->valueint;

        auto days = cJSON_GetObjectItemCaseSensitive(schedule_it, "days");
        if (cJSON_IsInvalid(days) || !cJSON_IsNumber(days))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].days.data = days->valueint;

        auto months = cJSON_GetObjectItemCaseSensitive(schedule_it, "months");
        if (cJSON_IsInvalid(months) || !cJSON_IsNumber(months))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].months.data = months->valueint;

        auto description = cJSON_GetObjectItemCaseSensitive(schedule_it, "description");
        if (cJSON_IsInvalid(description) || !cJSON_IsString(description))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].description = description->valuestring;


        auto status = cJSON_GetObjectItemCaseSensitive(schedule_it, "status");
        if (cJSON_IsInvalid(status) || !cJSON_IsNumber(status))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }
        schedules[schedule_count].status = hhg::app::status{status->valueint};

        auto zones = cJSON_GetObjectItemCaseSensitive(schedule_it, "zones");
        if (cJSON_IsInvalid(zones) || !cJSON_IsArray(minute))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            cJSON_Delete(root);
            return false;
        }

        uint8_t zone_count = 0;
        const cJSON *zone_it = NULL;
        cJSON_ArrayForEach(zone_it, zones)
        {

            auto name = cJSON_GetObjectItemCaseSensitive(zone_it, "name");
            if (cJSON_IsInvalid(name) || !cJSON_IsString(name))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].zones[zone_count].description = name->valuestring;

            auto description = cJSON_GetObjectItemCaseSensitive(zone_it, "description");
            if (cJSON_IsInvalid(description) || !cJSON_IsString(description))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].zones[zone_count].description = description->valuestring;

            auto relay_number = cJSON_GetObjectItemCaseSensitive(schedule_it, "relay_number");
            if (cJSON_IsInvalid(relay_number) || !cJSON_IsNumber(relay_number))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].zones[zone_count].relay_number = relay_number->valueint;

            auto watering_time = cJSON_GetObjectItemCaseSensitive(schedule_it, "watering_time");
            if (cJSON_IsInvalid(watering_time) || !cJSON_IsNumber(watering_time))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].zones[zone_count].watering_time = watering_time->valueint;

            auto weight = cJSON_GetObjectItemCaseSensitive(schedule_it, "weight");
            if (cJSON_IsInvalid(weight) || !cJSON_IsNumber(weight))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].zones[zone_count].weight = weight->valueint;

            auto status = cJSON_GetObjectItemCaseSensitive(schedule_it, "status");
            if (cJSON_IsInvalid(status) || !cJSON_IsNumber(status))
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                cJSON_Delete(root);
                return false;
            }
            schedules[schedule_count].status = hhg::app::status{status->valueint};


            zone_count++;
        }


        schedule_count++;
    }



    return false;
}

string<intf::data::FILE_SIZE> print(const schedule (&schedules)[HHGARDEN_SCHEDULES_SIZE], class error** error)
{
    string<intf::data::FILE_SIZE> ret;

    auto root = cJSON_CreateArray();
    if (root == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return {};
    }

    for(auto&& schedule_it : schedules)
    {

        auto schedule = cJSON_CreateObject();
        if (schedule == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }
        cJSON_AddItemToArray(root, schedule);

        auto minute = cJSON_AddNumberToObject(schedule, "minute", schedule_it.minute);
        if (minute == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }

        auto hour = cJSON_AddNumberToObject(schedule, "hour", schedule_it.hour);
        if (hour == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }


        auto days = cJSON_AddNumberToObject(schedule, "days", schedule_it.days.data);
        if (days == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }

        auto months = cJSON_AddNumberToObject(schedule, "months", schedule_it.months.data);
        if (months == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }


        auto description = cJSON_AddStringToObject(schedule, "description", schedule_it.description.c_str());
        if (description == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }

        auto status = cJSON_AddNumberToObject(schedule, "status", static_cast<uint8_t>(schedule_it.status));
        if (status == nullptr)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
            }
            return {};
        }


        for(auto&& zone_it : schedule_it.zones)
        {
            auto zone = cJSON_CreateObject();
            if (zone == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }
            cJSON_AddItemToArray(schedule, zone);

            auto name = cJSON_AddStringToObject(zone, "name", zone_it.description.c_str());
            if (name == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

            auto description = cJSON_AddStringToObject(zone, "description", zone_it.description.c_str());
            if (description == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

            auto relay_number = cJSON_AddNumberToObject(zone, "relay_number", zone_it.relay_number);
            if (relay_number == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

            auto watering_time = cJSON_AddNumberToObject(zone, "watering_time", zone_it.watering_time);
            if (watering_time == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

            auto weight = cJSON_AddNumberToObject(zone, "weight", zone_it.weight);
            if (weight == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

            auto status = cJSON_AddNumberToObject(zone, "status", static_cast<uint8_t>(zone_it.status));
            if (status == nullptr)
            {
                if(error)
                {
                    *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
                }
                return {};
            }

        }

    }

    char* string = cJSON_PrintUnformatted(root);
    if(string == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(cJSON_GetErrorPtr(), static_cast<uint8_t>(error_code::JSON_PARSE), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return {};
    }


    ret += string;

    free(string);

    return ret;
}




}
}

