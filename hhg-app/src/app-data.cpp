/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-app/app-data.hpp"
using hhg::iface::fs_io;
using hhg::iface::data_type;

#include "hhg-utils/hhg-utils.hpp"
using hhg::utils::crc32;

#include "osal/osal.hpp"
using namespace os;

#include "cJSON.h"

#include <time.h>

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_data::app_data(const fs_io::ptr& fsio) OS_NOEXCEPT
: fsio(fsio)
{

}

app_data::~app_data() = default;

inline os::exit app_data::init(os::error** error) OS_NOEXCEPT
{
	return load(nullptr, error);
}

void app_data::reset() OS_NOEXCEPT
{
	data = {};
}

os::exit app_data::store(os::error** error) const OS_NOEXCEPT
{
	data.crc = MAIGC;
	data.crc = crc32(reinterpret_cast<uint8_t *>(&data), sizeof(data));
	return fsio->write(data_type::DATA, reinterpret_cast<const uint8_t *>(&data), sizeof(data), error);
}


os::exit app_data::load(app_data::on_vesrion_change on_vesrion_change, os::error** error) OS_NOEXCEPT
{
	auto local_data = static_cast<class data*>(os_malloc(sizeof(class data)));
	if(local_data == nullptr)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_data::load() out of memory.", error_type::OS_ENOMEM);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}


	if(fsio->read(data_type::DATA, reinterpret_cast<uint8_t *>(local_data), sizeof(class data), error) == exit::KO)
	{
		return exit::KO;
	}

	uint32_t original_crc = local_data->crc;
	local_data->crc = MAIGC;
	uint32_t crc = crc32(reinterpret_cast<uint8_t *>(local_data), sizeof(class data));
	local_data->crc = original_crc;

	if(crc != original_crc)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_data::load() crc error.", error_type::OS_ERCRC);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(local_data->magic != MAIGC)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_data::load() magic number error.", error_type::OS_EBADF);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(on_vesrion_change)
	{
		on_vesrion_change(local_data->version);
	}

	data = *local_data;

	os_free(local_data);

	return exit::OK;
}

os::exit app_data::load_default(os::error** error) OS_NOEXCEPT
{
	reset();
	return store(error);
}

os::exit app_data::clear(os::error** error) const OS_NOEXCEPT
{
    return fsio->clear(data_type::DATA, error);
}

bool app_data::get_schedule(time_t timestamp, struct schedule& schedule) OS_NOEXCEPT
{
	tm* now = gmtime(&timestamp);

	for(uint8_t idx_schedule = 0; idx_schedule < HHG_SCHEDULES_SIZE; idx_schedule++)
	{
		auto&& current_schedule = data.schedules[idx_schedule];
		if(current_schedule.status != status::ACTIVE)
		{
			continue;
		}

		if( (current_schedule.hour == static_cast<uint8_t>(schedule::NOT_SET) && current_schedule.minute == static_cast<uint8_t>(schedule::NOT_SET)) || current_schedule.hour == static_cast<uint8_t>(schedule::NOT_SET) )
		{
			continue;
		}

		if( (current_schedule.hour == now->tm_hour && current_schedule.minute == now->tm_min)
			|| (current_schedule.hour == now->tm_hour && current_schedule.minute == static_cast<uint8_t>(schedule::NOT_SET) && now->tm_min == 0)
		)
		{
			if(
					(current_schedule.days.data == static_cast<uint8_t>(schedule::NOT_SET) && current_schedule.months.data == schedule::NOT_SET)
					|| (current_schedule.days.data & get_bit_day(now) && current_schedule.months.data == schedule::NOT_SET)
					|| (current_schedule.days.data == static_cast<uint8_t>(schedule::NOT_SET) && current_schedule.months.data & (1 << now->tm_mon))
					|| (current_schedule.days.data == now->tm_mday && current_schedule.months.data == now->tm_mon)

			)
			{
				schedule = current_schedule;
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

os::exit app_data::set_schedule(const char json_str[]) OS_NOEXCEPT
{
    if(json_str == NULL)
    {
        OS_LOG_ERROR(APP_TAG, "json_str nullptr");
        return exit::KO;
    }

	cJSON *root = cJSON_Parse(json_str);
	if (cJSON_IsInvalid(root) || !cJSON_IsObject(root))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read json root");
		return exit::KO;
	}

	const cJSON *id = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (cJSON_IsInvalid(id) || !cJSON_IsNumber(id))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read id");
        return exit::KO;
    }

	const cJSON *minute = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (cJSON_IsInvalid(minute) || !cJSON_IsNumber(minute))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read minute");
        return exit::KO;
    }

	const cJSON *hour = cJSON_GetObjectItemCaseSensitive(root, "hour");
    if (cJSON_IsInvalid(hour) || !cJSON_IsNumber(hour))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read hour");
        return exit::KO;
    }

	const cJSON *days = cJSON_GetObjectItemCaseSensitive(root, "days");
    if (cJSON_IsInvalid(days) || !cJSON_IsNumber(days))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read days");
        return exit::KO;
    }

	const cJSON *months = cJSON_GetObjectItemCaseSensitive(root, "months");
    if (cJSON_IsInvalid(months) || !cJSON_IsNumber(months))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read months");
        return exit::KO;
    }

	const cJSON *description = cJSON_GetObjectItemCaseSensitive(root, "description");
    if (cJSON_IsInvalid(description) || !cJSON_IsString(description))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read description");
        return exit::KO;
    }

	const cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "status");
	if (cJSON_IsInvalid(status) || !cJSON_IsNumber(status))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read status");
		return exit::KO;
	}

	uint8_t	idx = id->valueint;
	if(idx >= HHG_SCHEDULES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max scheduling index");
		return exit::KO;
	}

	schedule& s = data.schedules[idx];

	s.minute = minute->valueint;
	s.hour = hour->valueint;
	s.days.data = days->valueint;
	s.months.data = months->valueint;
	s.description = description->valuestring;
	s.status = static_cast<enum status>(status->valueint);

	return exit::OK;
}

os::exit app_data::set_zone(const char json_str[]) OS_NOEXCEPT
{
    if(json_str == NULL)
    {
        OS_LOG_ERROR(APP_TAG, "json_str nullptr");
        return exit::KO;
    }

	cJSON *root = cJSON_Parse(json_str);
	if (cJSON_IsInvalid(root) || !cJSON_IsObject(root))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read json root");
		return exit::KO;
	}

	const cJSON *id = cJSON_GetObjectItemCaseSensitive(root, "id");
    if (cJSON_IsInvalid(id) || !cJSON_IsNumber(id))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read id");
        return exit::KO;
    }

	const cJSON *id_schedule = cJSON_GetObjectItemCaseSensitive(root, "id_schedule");
    if (cJSON_IsInvalid(id) || !cJSON_IsNumber(id))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read id");
        return exit::KO;
    }

	const cJSON *description = cJSON_GetObjectItemCaseSensitive(root, "description");
    if (cJSON_IsInvalid(description) || !cJSON_IsString(description))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read description");
        return exit::KO;
    }

	const cJSON *relay_number = cJSON_GetObjectItemCaseSensitive(root, "relay_number");
    if (cJSON_IsInvalid(relay_number) || !cJSON_IsNumber(relay_number))
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Unable read relay_number");
        return exit::KO;
    }

	const cJSON *watering_time = cJSON_GetObjectItemCaseSensitive(root, "watering_time");
	if (cJSON_IsInvalid(watering_time) || !cJSON_IsNumber(watering_time))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read relay_number");
		return exit::KO;
	}

	const cJSON *weight = cJSON_GetObjectItemCaseSensitive(root, "weight");
	if (cJSON_IsInvalid(weight) || !cJSON_IsNumber(weight))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read weight");
		return exit::KO;
	}

	const cJSON *status = cJSON_GetObjectItemCaseSensitive(root, "status");
	if (cJSON_IsInvalid(status) || !cJSON_IsNumber(status))
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Unable read status");
		return exit::KO;
	}

	if(id_schedule->valueint >= HHG_SCHEDULES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max scheduling index");
		return exit::KO;
	}

	schedule& s = data.schedules[id_schedule->valueint];

	uint8_t	idx = id->valueint;

	if(idx > s.zones_len && idx >= HHG_ZONES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max zoning index");
		return exit::KO;
	}

	if(id->valueint == s.zones_len)
	{
		s.zones_len++;
	}

	s.zones[idx].description = description->valuestring;
	s.zones[idx].relay_number = relay_number->valueint;
	s.zones[idx].weight = weight->valueint;
	s.zones[idx].status = static_cast<enum status>(status->valueint);

	return exit::OK;
}

char *app_data::get_schedule(uint8_t id) OS_NOEXCEPT
{
	if(id >= HHG_SCHEDULES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max scheduling index");
		return nullptr;
	}

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
    	OS_LOG_ERROR(APP_TAG, "Malloc fail for root");
        return nullptr;
    }

	if (cJSON_AddNumberToObject(root, "id", id) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for id");
		return nullptr;
	}

    if (cJSON_AddNumberToObject(root, "minute", data.schedules[id].minute) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for minute");
        return nullptr;
    }

    if (cJSON_AddNumberToObject(root, "hour", data.schedules[id].hour) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for hour");
        return nullptr;
    }

    if (cJSON_AddNumberToObject(root, "days", data.schedules[id].days.data) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for days");
        return nullptr;
    }

    if (cJSON_AddNumberToObject(root, "months", data.schedules[id].months.data) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for months");
        return nullptr;
    }

    if (cJSON_AddStringToObject(root, "description", data.schedules[id].description.c_str()) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for description");
        return nullptr;
    }

    if (cJSON_AddNumberToObject(root, "status", static_cast<uint8_t>(data.schedules[id].status)) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for status");
        return nullptr;
    }

    char *ret = cJSON_PrintUnformatted(root);
	if(ret == NULL)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for ret");
		return nullptr;
	}

	cJSON_Delete(root);

	return ret;
}

char *app_data::get_zone(uint8_t id_schedule, uint8_t id) OS_NOEXCEPT
{
	if(id >= HHG_ZONES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max scheduling index");
		return nullptr;
	}

	if(id >= data.schedules[id_schedule].zones_len + 1 || id >= HHG_ZONES_SIZE)
	{
		OS_LOG_ERROR(APP_TAG, "Out of max zoning index");
		return nullptr;
	}

	cJSON *root = cJSON_CreateObject();
	if (root == NULL)
	{
		OS_LOG_ERROR(APP_TAG, "Malloc fail for root");
		return nullptr;
	}

	if (cJSON_AddNumberToObject(root, "id_schedule", id_schedule) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for id_schedule");
		return nullptr;
	}


	if (cJSON_AddNumberToObject(root, "id", id) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for id");
		return nullptr;
	}

	if (cJSON_AddStringToObject(root, "description", data.schedules[id_schedule].zones[id].description.c_str()) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for description");
		return nullptr;
	}

	if (cJSON_AddNumberToObject(root, "relay_number", data.schedules[id_schedule].zones[id].relay_number) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for relay_number");
		return nullptr;
	}

	if (cJSON_AddNumberToObject(root, "watering_time", data.schedules[id_schedule].zones[id].watering_time) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for watering_time");
		return nullptr;
	}

	if (cJSON_AddNumberToObject(root, "weight", data.schedules[id_schedule].zones[id].weight) == nullptr)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for weight");
		return nullptr;
	}

    if (cJSON_AddNumberToObject(root, "status", static_cast<uint8_t>(data.schedules[id].zones[id].weight)) == nullptr)
    {
        cJSON_Delete(root);
        OS_LOG_ERROR(APP_TAG, "Malloc fail for status");
        return nullptr;
    }


	char *ret = cJSON_PrintUnformatted(root);
	if(ret == NULL)
	{
		cJSON_Delete(root);
		OS_LOG_ERROR(APP_TAG, "Malloc fail for ret");
		return nullptr;
	}

	cJSON_Delete(root);

	return ret;
}

uint8_t app_data::get_bit_day(const tm* now) const OS_NOEXCEPT
{
	if(now == nullptr)
	{
		return static_cast<uint8_t>(schedule::NOT_SET);
	}

	switch(now->tm_mday)
	{
		case 0: //sun
			return (1 << 6);
		case 1: //mon
			return (1 << 0);
		case 2: //tue
			return (1 << 1);
		case 3: //wen
			return (1 << 2);
		case 4: //thu
			return (1 << 3);
		case 5: //fri
			return (1 << 4);
		case 6: //sat
			return (1 << 5);
	}
	return static_cast<uint8_t>(schedule::NOT_SET);
}

} /* namespace driver */
} /* namespace hhg */
