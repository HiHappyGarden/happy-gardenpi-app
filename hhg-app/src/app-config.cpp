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

#include "hhg-app/app-config.hpp"
using namespace os;
using hhg::iface::fs_io;
using hhg::iface::data_type;

#include "hhg-utils/hhg-utils.hpp"
using hhg::utils::crc32;

#include "hhg-config.h"
#include "cJSON.h"

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_config::app_config(const fs_io::ptr& fsio) OS_NOEXCEPT
: fsio(fsio)
{

}

app_config::~app_config() = default;

inline os::exit app_config::init(error** error) OS_NOEXCEPT
{
	return load(nullptr, error);
}

os::exit app_config::set_serial(const char serial[]) OS_NOEXCEPT
{
	if(serial)
		config.serial += serial;
	else
		config.serial = "";

	error* error = nullptr;
	if(fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

os::exit app_config::set_descr(const char descr[]) OS_NOEXCEPT
{
	if(descr)
		config.descr += descr;
	else
		config.descr = "";

	error* error = nullptr;
	if(fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

const char* app_config::get_version() const OS_NOEXCEPT
{
	static string<16> ret;
	if(ret.length() == 0)
	{
		snprintf(ret.c_str(), ret.size(), "%s", HHG_VER);
	}
	return ret.c_str();
}

os::exit app_config::store(error** error) const OS_NOEXCEPT
{
	config.crc = MAIGC;
	config.crc = crc32(reinterpret_cast<uint8_t *>(&config), sizeof(config));
	return fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t *>(&config), sizeof(config), error);
}

os::exit app_config::load(app_config::on_vesrion_change on_version_change, error** error) OS_NOEXCEPT
{
	auto local_config = new class config;
    if(local_config == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("app_config::load() out of memory.", error_type::OS_ENOMEM);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }



	if(fsio->read(data_type::CONFIG, reinterpret_cast<uint8_t *>(local_config), sizeof(class config), error) == exit::KO)
	{
		return exit::KO;
	}

	uint32_t original_crc = local_config->crc;
	local_config->crc = MAIGC;
	uint32_t crc = crc32(reinterpret_cast<uint8_t *>(local_config), sizeof(class config));
	local_config->crc = original_crc;

	if(crc != original_crc)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() crc error.", error_type::OS_ERCRC);
			OS_ERROR_PTR_SET_POSITION(*error);
            delete local_config;
		}
		return exit::KO;
	}

	if(local_config->magic != MAIGC)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() magic number error.", error_type::OS_EBADF);
			OS_ERROR_PTR_SET_POSITION(*error);
            delete local_config;
		}
		return exit::KO;
	}

	if(on_version_change)
	{
		on_version_change(local_config->version);
	}

	config = *local_config;

    delete local_config;

	return exit::OK;
}

os::exit app_config::load_default(os::error **error) OS_NOEXCEPT
{
    struct config config_default;
	config = config_default;
	return store(error);
}

os::exit app_config::clear(os::error** error) const OS_NOEXCEPT
{
    return fsio->clear(data_type::CONFIG, error);
}

    const char *app_config::get_config(bool unformatted) const OS_NOEXCEPT
    {
        cJSON *root = cJSON_CreateObject();
        if (root == nullptr)
        {
            OS_LOG_ERROR(APP_TAG, "Malloc fail for root");
            return nullptr;
        }

        if (cJSON_AddStringToObject(root, "serial", config.serial.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for serial");
            return nullptr;
        }

        if (cJSON_AddStringToObject(root, "descr", config.descr.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for descr");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "zones_size", config.zones_size) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for zones_size");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "users_len", config.users_len) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for users_len");
            return nullptr;
        }


        auto users = cJSON_AddArrayToObject(root, "users");
        if (users == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for users");
            return nullptr;
        }

        for (uint8_t i = 0; i < config.users_len && i < user::MAX_USERS; ++i)
        {
            auto user = cJSON_CreateObject();

            if (cJSON_AddStringToObject(user, "user", config.users[i].user.c_str()) == nullptr)
            {
                cJSON_Delete(root);
                cJSON_Delete(user);
                OS_LOG_ERROR(APP_TAG, "Malloc fail for users[i].user");
                return nullptr;
            }

            if (cJSON_AddStringToObject(user, "passwd", config.users[i].passwd.c_str()) == nullptr)
            {
                cJSON_Delete(root);
                cJSON_Delete(user);
                OS_LOG_ERROR(APP_TAG, "Malloc fail for users[i].passwd");
                return nullptr;
            }

            cJSON_AddItemToArray(users, user);
        }

        if (cJSON_AddStringToObject(root, "wifi_ssid", config.wifi_ssid.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for wifi_ssid");
            return nullptr;
        }

        if (cJSON_AddStringToObject(root, "wifi_passwd", config.wifi_passwd.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for wifi_passwd");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "wifi_auth", config.wifi_auth) == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for wifi_auth");
            return nullptr;
        }

        char *ret = nullptr;
        if(unformatted)
        {
            ret = cJSON_PrintUnformatted(root);
        }
        else
        {
            ret = cJSON_Print(root);
        }

        if(ret == nullptr)
        {
            cJSON_Delete(root);
            OS_LOG_ERROR(APP_TAG, "Malloc fail for ret");
            return nullptr;
        }

        cJSON_Delete(root);

        return ret;
    }

} /* namespace driver */
} /* namespace hhg */
