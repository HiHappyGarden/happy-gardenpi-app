/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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

#include "osal/osal.hpp"

#include "hhg-iface/initializable.hpp"

#include <stdint.h>

namespace hhg::iface
{
inline namespace v1
{

enum class data_type : uint8_t
{
	CONFIG,
	DATA,
};

struct fs_io : public initializable
{
	using ptr = os::unique_ptr<hhg::iface::fs_io>;

	~fs_io() override = default;

	virtual os::exit write(data_type type, const uint8_t* data, size_t size, os::error** error) const OSAL_NOEXCEPT = 0;

	virtual os::exit read(data_type type, uint8_t* data, size_t size, os::error** error) const OSAL_NOEXCEPT = 0;

    virtual os::exit clear(data_type type, os::error** error) const OSAL_NOEXCEPT = 0;
};

}
}
