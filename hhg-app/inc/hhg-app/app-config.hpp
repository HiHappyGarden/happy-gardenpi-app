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

#pragma once

#include "hhg-iface/fs-io.hpp"
#include "hhg-iface/initializable.hpp"
#include "hhg-iface/file-version.hpp"

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif

#include <stdint.h>

namespace hhg::app
{
inline namespace v1
{


class app_config final : public hhg::iface::initializable
{
public:
    struct alignas(4) user final
    {
        static constexpr uint8_t MAX_USERS = 2;
        static constexpr uint8_t ADMIN = 0;
        static struct user EMPTY;

        os::string<32> user;
        os::string<32> passwd;

        inline bool is_empty() const OSAL_NOEXCEPT
        {
            return user.length() == 0 && passwd.length() == 0;
        }
    };

private:

    constexpr static const uint32_t MAGIC = 0x2E'43'4E'46;
	constexpr static const uint8_t VERSION = 1;

	const hhg::iface::fs_io::ptr& fs_io;

	mutable struct alignas(64) config final : public hhg::iface::file_version {
        inline config() OSAL_NOEXCEPT: file_version{MAGIC, VERSION} {}

        os::string<16> serial;
        os::string<128> descr;
        uint8_t zones_size = HHG_ZONES_SIZE;
        uint8_t users_len = 0;
        user users[user::MAX_USERS];
        struct
        {
            os::string<32> ssid;
            os::string<64> passwd;
            uint32_t auth = 0;
            bool enabled = true;
        }wifi;
        int16_t timezone = HHG_TIMEZONE; //<! in minutes
        bool daylight_saving_time = HHG_DAYLIGHT_SAVING_TIME;
        uint32_t crc = MAGIC;
	} config;

public:

	using on_vesrion_change = void (*)(uint8_t version);

	explicit app_config(const hhg::iface::fs_io::ptr& fsio) OSAL_NOEXCEPT;
	~app_config() override;
	OSAL_NO_COPY_NO_MOVE(app_config)

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

	inline const char* get_serial() const OSAL_NOEXCEPT
	{
		return config.serial.c_str();
	}

	os::exit set_serial(const char serial[]) OSAL_NOEXCEPT;

	inline const char* get_descr() const OSAL_NOEXCEPT
	{
		return config.descr.c_str();
	}

	inline uint8_t get_zones_size() const OSAL_NOEXCEPT
	{
		return config.zones_size;
	}

    inline void set_wifi_ssid(const char* wifi_ssid) OSAL_NOEXCEPT
    {
        this->config.wifi.ssid = wifi_ssid;
    }

    inline const os::string<32>& get_wifi_ssid() const OSAL_NOEXCEPT
    {
        return this->config.wifi.ssid;
    }

    inline void set_wifi_passwd(const char* wifi_passwd) OSAL_NOEXCEPT
    {
        this->config.wifi.passwd = wifi_passwd;
    }

    inline const os::string<64>& get_wifi_passwd() const OSAL_NOEXCEPT
    {
        return this->config.wifi.passwd;
    }

    inline void set_wifi_auth(uint32_t wifi_auth) OSAL_NOEXCEPT
    {
        this->config.wifi.auth = wifi_auth;
    }

    inline uint32_t get_wifi_auth() const OSAL_NOEXCEPT
    {
        return this->config.wifi.auth;
    }

    inline void set_wifi_enabled(bool enabled) OSAL_NOEXCEPT
    {
        this->config.wifi.enabled = enabled;
    }

    inline bool is_wifi_enabled() const OSAL_NOEXCEPT
    {
        return this->config.wifi.enabled;
    }

    os::exit set_user(uint8_t idx, const char* user, const char* passwd);

    inline const user& get_user(uint8_t idx) const OSAL_NOEXCEPT
    {
        if(idx >= user::MAX_USERS || idx > config.users_len)
        {
            return user::EMPTY;
        }
        return this->config.users[idx];
    }

    inline void set_timezone(int16_t timezone_in_minutes) OSAL_NOEXCEPT
    {
        config.timezone = timezone_in_minutes;
    }

    inline int16_t get_timezone() const OSAL_NOEXCEPT
    {
        return config.timezone;
    }

    inline void set_daylight_saving_time(bool daylight_saving_time) OSAL_NOEXCEPT
    {
        config.daylight_saving_time = daylight_saving_time;
    }

    inline bool get_daylight_saving_time() const OSAL_NOEXCEPT
    {
        return config.daylight_saving_time;
    }

    os::pair<os::exit, user> set_auth(const os::string<32>& user, const os::string<32>& passwd);

    os::pair<os::exit, user> set_auth_remote(const os::string<32>& user, const os::string<32>& passwd);

    os::exit set_descr(const char descr[]) OSAL_NOEXCEPT;

	const char* get_version() OSAL_NOEXCEPT;

	os::exit store(os::error** error) const OSAL_NOEXCEPT;

	os::exit load(app_config::on_vesrion_change on_version_change, os::error** error) OSAL_NOEXCEPT;

	os::exit load_default(os::error **error) OSAL_NOEXCEPT;

    os::exit clear(os::error** error) const OSAL_NOEXCEPT;

    const char* get_config(bool unformatted = true) const OSAL_NOEXCEPT;

};

}
}

