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

#include "hhg-iface/initializable.hpp"
#include "hhg-iface/fsio.hpp"

#include <stdint.h>

namespace hhg::app
{
inline namespace v1
{


class app_config final : public hhg::iface::initializable
{
	const hhg::iface::fsio::ptr& fsio;

	struct config
	{
		os::string<16> 	serial;
		os::string<128> descr;
		uint8_t version_major = HHG_VER_MAJOR;
		uint8_t version_minor = HHG_VER_MINOR;
		uint8_t version_patch = HHG_VER_PATCH;
	} config;

public:
	explicit app_config(const hhg::iface::fsio::ptr& fsio) OS_NOEXCEPT;
	~app_config();
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

	os::exit set_descr(const char descr[]) OS_NOEXCEPT;

	const char* get_version() const OS_NOEXCEPT;

	os::exit store(os::error** error) const OS_NOEXCEPT;

};

}
}

