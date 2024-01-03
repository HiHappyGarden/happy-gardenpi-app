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

#include "hhg-intf/io.hpp"

namespace hhg::driver
{
inline namespace v1
{


class stm32_io final : public hhg::intf::io
{
	on_read on_read_callback = nullptr;

	static inline stm32_io* singleton = nullptr;
public:
	enum source
	{
		LPUART
	}source;

	stm32_io();
	~stm32_io() OS_NOEXCEPT override;
	OS_NO_COPY_NO_MOVE(stm32_io)

	inline enum source get_source() const OS_NOEXCEPT
	{
		return source;
	}

	os::exit init(os::error** error) OS_NOEXCEPT override;

	void set_on_read(on_read on_read_callback) OS_NOEXCEPT override;

    os::exit write(const uint8_t data[], uint16_t size) const OS_NOEXCEPT override;

};

}
}
