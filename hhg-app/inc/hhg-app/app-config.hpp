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
	constexpr static const uint32_t MAIGC = 0xf86c2975;
	constexpr static const uint8_t VERSION = 1;

	const hhg::iface::fs_io::ptr& fs_io;

	struct alignas(2) user final
	{

		static constexpr uint8_t MAX_USERS = 2;

		os::string<128> user;
		os::string<64> passwd;

	};

	mutable struct alignas(2) config final : public hhg::iface::file_version
	{
		inline config() OS_NOEXCEPT : file_version{MAIGC, VERSION} {}
		os::string<16> serial;
		os::string<128> descr;
		uint8_t zones_size = HHG_ZONES_SIZE;
        uint8_t users_len = 0;
		user users[user::MAX_USERS];
        os::string<32> wifi_ssid;
        os::string<64> wifi_passwd;
        uint32_t wifi_auth = 0;
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
        this->config.wifi_ssid = wifi_ssid;
    }

    inline const os::string<32>& get_wifi_ssid() const OS_NOEXCEPT
    {
        return this->config.wifi_ssid;
    }

    inline void set_wifi_passwd(const char* wifi_passwd) OS_NOEXCEPT
    {
        this->config.wifi_passwd = wifi_passwd;
    }

    inline const os::string<64>& get_wifi_passwd() const OS_NOEXCEPT
    {
        return this->config.wifi_passwd;
    }

    inline void set_wifi_auth(uint32_t wifi_auth) OS_NOEXCEPT
    {
        this->config.wifi_auth = wifi_auth;
    }

    inline uint32_t get_wifi_auth() const OS_NOEXCEPT
    {
        return this->config.wifi_auth;
    }

    os::exit set_descr(const char descr[]) OS_NOEXCEPT;

	const char* get_version() const OS_NOEXCEPT;

	os::exit store(os::error** error) const OS_NOEXCEPT;

	os::exit load(app_config::on_vesrion_change on_version_change, os::error** error) OS_NOEXCEPT;

	os::exit load_default(os::error **error) OS_NOEXCEPT;

    os::exit clear(os::error** error) const OS_NOEXCEPT;

    const char* get_config(bool unformatted = true) const OS_NOEXCEPT;

};

}
}

