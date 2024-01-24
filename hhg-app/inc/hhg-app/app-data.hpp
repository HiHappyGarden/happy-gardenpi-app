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

#include <stdint.h>

namespace hhg::app
{
inline namespace v1
{


class app_data final : public hhg::iface::initializable
{
	constexpr static const uint32_t MAIGC = 0xd2b9b032;
	constexpr static const uint8_t VERSION = 1;

	const hhg::iface::fs_io::ptr& fsio;

	mutable struct alignas(64) data final : public hhg::iface::file_version
	{
		inline data() : file_version{MAIGC, VERSION} {}
		uint32_t crc = MAIGC;
	} data;

public:

	using on_vesrion_change = void (*)(uint8_t version);

	explicit app_data(const hhg::iface::fs_io::ptr& fsio) OS_NOEXCEPT;
	~app_data();
	OS_NO_COPY_NO_MOVE(app_data)

	os::exit init(os::error** error) OS_NOEXCEPT override;

	os::exit store(os::error** error) const OS_NOEXCEPT;

	os::exit load(app_data::on_vesrion_change on_vesrion_change, os::error** error) OS_NOEXCEPT;

	os::exit load_defaut(os::error** error) OS_NOEXCEPT;
};

}
}

