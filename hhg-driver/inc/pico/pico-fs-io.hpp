/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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
#include <hardware/flash.h>

namespace hhg::driver
{
inline namespace v1
{

class pico_fs_io final : public iface::v1::fs_io
{
	static constexpr uint32_t START_STORE_ADDRESS = PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE;

public:


	static uint64_t const check_data;

	pico_fs_io() OSAL_NOEXCEPT;
	~pico_fs_io() OSAL_NOEXCEPT override;
    OSAL_NO_COPY_NO_MOVE(pico_fs_io);

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

	os::exit write(iface::v1::data_type type, const uint8_t* data, size_t size, os::error** error) const OSAL_NOEXCEPT override;

	os::exit read(iface::v1::data_type type, uint8_t* data, size_t size, os::error** error) const OSAL_NOEXCEPT override;

    os::exit clear(iface::data_type type, os::error** error) const OSAL_NOEXCEPT override;
private:

    static inline uint32_t get_offset(iface::v1::data_type type) OSAL_NOEXCEPT
    {
        return type == iface::v1::data_type::DATA ? FLASH_SECTOR_SIZE / 2 : 0;
    }

};

}
}

