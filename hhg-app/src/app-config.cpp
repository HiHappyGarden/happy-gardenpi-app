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

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif
#include "cJSON.h"
#include "md5.h"

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_config::user app_config::user::EMPTY = {};

app_config::app_config(const fs_io::ptr& fsio) OSAL_NOEXCEPT
: fs_io(fsio)
{

}

app_config::~app_config() = default;

inline os::exit app_config::init(error** error) OSAL_NOEXCEPT
{
	return load(nullptr, error);
}

os::exit app_config::set_serial(const char serial[]) OSAL_NOEXCEPT
{
	if(serial)
		config.serial += serial;
	else
		config.serial = "";

	error* error = nullptr;
	if(fs_io->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

os::exit app_config::set_user(uint8_t idx, const char *user, const char *passwd) OSAL_NOEXCEPT
{
    auto user_len = strlen(user);
    auto passwd_len = strlen(passwd);
    if(idx >= user::MAX_USERS || idx > config.users_len || user_len == 0 || user_len > 32 || passwd_len == 0 || passwd_len == 32)
    {
        return os::exit::KO;
    }

    char digest[33];
    uint8_t hash[16];

    // Write the calculated hash to `hash`
    MD5::hash(passwd, hash);
    // Retrieve the hex-encoded digest
    MD5::digest(hash, digest);

    if(config.users[idx].is_empty())
    {
        config.users_len++;
        config.users[idx].user = user;
        config.users[idx].passwd = digest;
    }
    else
    {
        if(config.users[idx].user != user)
        {
            return os::exit::KO;
        }
        config.users[idx].passwd = digest;
    }

    return os::exit::OK;
}


os::pair<os::exit, app_config::user> app_config::set_auth(const os::string<32>& user, const os::string<32>& passwd)
{
    for(uint8_t i = 0; i < config.users_len && i <  user::MAX_USERS; i++)
    {
        auto&& [u, p] = config.users[i];

        char digest[33];
        uint8_t hash[16];

        // Write the calculated hash to `hash`
        MD5::hash(passwd.c_str(), hash);
        // Retrieve the hex-encoded digest
        MD5::digest(hash, digest);

        if(u == user && p == digest)
        {
            return {exit::OK, {u, p}};
        }
    }

    return {exit::KO, {}};
}

os::pair<os::exit, app_config::user> app_config::set_auth_remote(const os::string<32>& user, const os::string<32>& passwd)
{
    for(uint8_t i = 0; i < config.users_len && i <  user::MAX_USERS; i++)
    {
        auto&& [u, p] = config.users[i];
        if(u == user && p == passwd)
        {
            return {exit::OK, {u, p}};
        }
    }

    return {exit::KO, {}};
}


os::exit app_config::set_descr(const char descr[]) OSAL_NOEXCEPT
{
	if(descr)
		config.descr += descr;
	else
		config.descr = "";

	error* error = nullptr;
	if(fs_io->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

const char* app_config::get_version() OSAL_NOEXCEPT
{
	static string<16> ret;
	if(ret.length() == 0)
	{
		snprintf(ret.c_str(), ret.size(), "%s", HHG_VER);
	}
	return ret.c_str();
}

os::exit app_config::store(error** error) const OSAL_NOEXCEPT
{
	config.crc = MAGIC;
	config.crc = crc32(reinterpret_cast<uint8_t *>(&config), sizeof(config));
	return fs_io->write(data_type::CONFIG, reinterpret_cast<const uint8_t *>(&config), sizeof(config), error);
}

os::exit app_config::load(app_config::on_vesrion_change on_version_change, error** error) OSAL_NOEXCEPT
{
	auto local_config = new class config;
    if(local_config == nullptr)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("app_config::load() out of memory.", error_type::OS_ENOMEM);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }



	if(fs_io->read(data_type::CONFIG, reinterpret_cast<uint8_t *>(local_config), sizeof(class config), error) == exit::KO)
	{
		return exit::KO;
	}

	uint32_t original_crc = local_config->crc;
	local_config->crc = MAGIC;
	uint32_t crc = crc32(reinterpret_cast<uint8_t *>(local_config), sizeof(class config));
	local_config->crc = original_crc;

	if(crc != original_crc)
	{
		if(error)
		{
			*error = OSAL_ERROR_BUILD("app_config::load() crc error.", error_type::OS_ERCRC);
			OSAL_ERROR_PTR_SET_POSITION(*error);
            delete local_config;
		}
		return exit::KO;
	}

	if(local_config->magic != MAGIC)
	{
		if(error)
		{
			*error = OSAL_ERROR_BUILD("app_config::load() magic number error.", error_type::OS_EBADF);
			OSAL_ERROR_PTR_SET_POSITION(*error);
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

os::exit app_config::load_default(os::error **error) OSAL_NOEXCEPT
{
#if !defined(HHG_ADMIN_USER) || !defined(HHG_ADMIN_PASSWD)
#error Admin user and password it's mandatory'
#endif
    char digest[33];
    uint8_t hash[16];

    // Write the calculated hash to `hash`
    MD5::hash(HHG_ADMIN_PASSWD, hash);
    // Retrieve the hex-encoded digest
    MD5::digest(hash, digest);


    struct config config_default;

    config_default.users[0].user = HHG_ADMIN_USER;
    config_default.users[0].passwd = digest;
    config_default.users_len = 1;

#if defined(HHG_USER) && defined(HHG_PASSWD)
    if(app_config::user::MAX_USERS > 1)
    {
        // Write the calculated hash to `hash`
        MD5::hash(HHG_PASSWD, hash);
        // Retrieve the hex-encoded digest
        MD5::digest(hash, digest);

        config_default.users[1].user = HHG_USER;
        config_default.users[1].passwd = digest;
        config_default.users_len++;
    }
#endif

	config = config_default;
	return store(error);
}

os::exit app_config::clear(os::error** error) const OSAL_NOEXCEPT
{
    return fs_io->clear(data_type::CONFIG, error);
}

    const char *app_config::get_config(bool unformatted) const OSAL_NOEXCEPT
    {
        cJSON *root = cJSON_CreateObject();
        if (root == nullptr)
        {
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for root");
            return nullptr;
        }

        if (cJSON_AddStringToObject(root, "serial", config.serial.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for serial");
            return nullptr;
        }

        if (cJSON_AddStringToObject(root, "descr", config.descr.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for descr");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "zones_size", config.zones_size) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for zones_size");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "users_len", config.users_len) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for users_len");
            return nullptr;
        }


        auto users = cJSON_AddArrayToObject(root, "users");
        if (users == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for users");
            return nullptr;
        }

        for (uint8_t i = 0; i < config.users_len && i < user::MAX_USERS; ++i)
        {
            auto user = cJSON_CreateObject();
            if(user == nullptr)
            {
                cJSON_Delete(root);
                OSAL_LOG_ERROR(APP_TAG, "Malloc fail for user");
                return nullptr;
            }

            if (cJSON_AddStringToObject(user, "user", config.users[i].user.c_str()) == nullptr)
            {
                cJSON_Delete(root);
                cJSON_Delete(user);
                OSAL_LOG_ERROR(APP_TAG, "Malloc fail for users[i].user");
                return nullptr;
            }

            if (cJSON_AddStringToObject(user, "passwd", config.users[i].passwd.c_str()) == nullptr)
            {
                cJSON_Delete(root);
                cJSON_Delete(user);
                OSAL_LOG_ERROR(APP_TAG, "Malloc fail for users[i].passwd");
                return nullptr;
            }

            cJSON_AddItemToArray(users, user);
        }

        auto wifi = cJSON_CreateObject();
        if(wifi == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for user");
            return nullptr;
        }

        if (cJSON_AddStringToObject(wifi, "ssid", config.wifi.ssid.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            cJSON_Delete(wifi);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for wifi.ssid");
            return nullptr;
        }

        if (cJSON_AddStringToObject(wifi, "passwd", config.wifi.passwd.c_str()) == nullptr)
        {
            cJSON_Delete(root);
            cJSON_Delete(wifi);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for wifi.passwd");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(wifi, "auth", config.wifi.auth) == nullptr)
        {
            cJSON_Delete(root);
            cJSON_Delete(wifi);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for wifi.auth");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(wifi, "enabled", config.wifi.enabled) == nullptr)
        {
            cJSON_Delete(root);
            cJSON_Delete(wifi);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for wifi.enabled");
            return nullptr;
        }
        cJSON_AddItemToObject(root, "wifi", wifi);

        if (cJSON_AddNumberToObject(root, "timezone", config.timezone) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for timezone");
            return nullptr;
        }

        if (cJSON_AddNumberToObject(root, "daylight_saving_time", config.daylight_saving_time) == nullptr)
        {
            cJSON_Delete(root);
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for daylight_saving_time");
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
            OSAL_LOG_ERROR(APP_TAG, "Malloc fail for ret");
            return nullptr;
        }

        cJSON_Delete(root);

        return ret;
    }

} /* namespace driver */
} /* namespace hhg */
