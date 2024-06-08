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
    struct alignas(2) user final
    {
        static constexpr uint8_t MAX_USERS = 2;
        static constexpr uint8_t ADMIN = 0;
        static struct user EMPTY;

        os::string<32> user;
        os::string<32> passwd;

        inline bool is_empty() const OS_NOEXCEPT
        {
            return user.length() == 0 && passwd.length() == 0;
        }
    };

private:

    constexpr static const uint32_t MAIGC = 0xf86c2975;
	constexpr static const uint8_t VERSION = 1;

	const hhg::iface::fs_io::ptr& fs_io;

	mutable struct alignas(2) config final : public hhg::iface::file_version {
        inline config() OS_NOEXCEPT: file_version{MAIGC, VERSION} {}

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
        uint32_t crc = MAIGC;
	} config;

public:

	using on_vesrion_change = void (*)(uint8_t version);

	explicit app_config(const hhg::iface::fs_io::ptr& fsio) OS_NOEXCEPT;
	~app_config() override;
	OS_NO_COPY_NO_MOVE(app_config)

	os::exit init(os::error** error) OS_NOEXCEPT override;

	inline const char* get_serial() const OS_NOEXCEPT
	{
		return config.serial.c_str();
	}

	os::exit set_serial(const char serial[]) OS_NOEXCEPT;

	inline const char* get_descr() const OS_NOEXCEPT
	{
		return config.descr.c_str();
	}

	inline uint8_t get_zones_size() const OS_NOEXCEPT
	{
		return config.zones_size;
	}

    inline void set_wifi_ssid(const char* wifi_ssid) OS_NOEXCEPT
    {
        this->config.wifi.ssid = wifi_ssid;
    }

    inline const os::string<32>& get_wifi_ssid() const OS_NOEXCEPT
    {
        return this->config.wifi.ssid;
    }

    inline void set_wifi_passwd(const char* wifi_passwd) OS_NOEXCEPT
    {
        this->config.wifi.passwd = wifi_passwd;
    }

    inline const os::string<64>& get_wifi_passwd() const OS_NOEXCEPT
    {
        return this->config.wifi.passwd;
    }

    inline void set_wifi_auth(uint32_t wifi_auth) OS_NOEXCEPT
    {
        this->config.wifi.auth = wifi_auth;
    }

    inline uint32_t get_wifi_auth() const OS_NOEXCEPT
    {
        return this->config.wifi.auth;
    }

    inline void set_wifi_enabled(bool enabled) OS_NOEXCEPT
    {
        this->config.wifi.enabled = enabled;
    }

    inline bool is_wifi_enabled() const OS_NOEXCEPT
    {
        return this->config.wifi.enabled;
    }

    os::exit set_user(uint8_t idx, const char* user, const char* passwd);

    inline const user& get_user(uint8_t idx) const OS_NOEXCEPT
    {
        if(idx >= user::MAX_USERS || idx > config.users_len)
        {
            return user::EMPTY;
        }
        return this->config.users[idx];
    }

    os::pair<os::exit, user> set_auth(const os::string<32>& user, const os::string<32>& passwd);

    os::pair<os::exit, user> set_remote_auth(const os::string<32>& user, const os::string<32>& passwd);

    os::exit set_descr(const char descr[]) OS_NOEXCEPT;

	const char* get_version() OS_NOEXCEPT;

	os::exit store(os::error** error) const OS_NOEXCEPT;

	os::exit load(app_config::on_vesrion_change on_version_change, os::error** error) OS_NOEXCEPT;

	os::exit load_default(os::error **error) OS_NOEXCEPT;

    os::exit clear(os::error** error) const OS_NOEXCEPT;

    const char* get_config(bool unformatted = true) const OS_NOEXCEPT;

};

}
}

