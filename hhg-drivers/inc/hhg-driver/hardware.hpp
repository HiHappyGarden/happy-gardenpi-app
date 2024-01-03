/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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
#include "hhg-iface/io.hpp"

namespace hhg::driver
{
inline namespace v1
{

using io_ptr = hhg::iface::io::ptr;

class hardware final : public hhg::iface::initializable
{
	io_ptr io;
public:
	explicit hardware(class os::error** error) OS_NOEXCEPT;
	~hardware() = default;
	OS_NO_COPY_NO_MOVE(hardware)

	inline const io_ptr& get_io() const OS_NOEXCEPT
	{
		return io;
	}

    os::exit init(os::error** error) override OS_NOEXCEPT;

    const os::string<128>& get_info() OS_NOEXCEPT;

    const os::string<128>& get_version() OS_NOEXCEPT;
};

}
}
