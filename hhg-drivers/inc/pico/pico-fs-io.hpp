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

#include "hhg-iface/fs-io.hpp"


namespace hhg::driver
{
inline namespace v1
{

class pico_fsio final : public iface::v1::fs_io
{
	uint32_t start_flash_address = 0;
	uint32_t end_flash_address = 0;

public:


	static uint64_t const check_data;

	pico_fsio(uint32_t start_flash_address, uint32_t end_flash_address) OS_NOEXCEPT;
	virtual ~pico_fsio();

	os::exit init(os::error** error) OS_NOEXCEPT override;

	os::exit write(iface::v1::data_type type, const uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;

	os::exit read(iface::v1::data_type type, uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;

private:


};

}
}

