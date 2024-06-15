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


#include "pico/pico-fs-io.hpp"
using namespace hhg::iface;
using namespace os;

#include "hhg-utils/hhg-utils.hpp"
using namespace hhg::utils;

#include <hardware/sync.h>
#include <hardware/flash.h>

namespace hhg::driver
{
namespace v1
{

uint64_t const pico_fs_io::check_data = 0x0102030404030201;

pico_fs_io::pico_fs_io() = default;

pico_fs_io::~pico_fs_io() = default;

os::exit pico_fs_io::init(error** error) OSAL_NOEXCEPT
{
	return exit::OK;
}

os::exit pico_fs_io::write(data_type type, const uint8_t* data, size_t size, error** error) const OSAL_NOEXCEPT
{
    if(data == nullptr)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "pico_fs_io::write() data == null", error_type::OS_ENOENT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    if(size > FLASH_SECTOR_SIZE / 2)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "pico_fs_io::write() too much data to write", error_type::OS_ENOENT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    //TODO: remove dynamic allocation but before increase stack space
    auto data_sector = new uint8_t[FLASH_SECTOR_SIZE];
    if(data_sector == nullptr)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "pico_fs_io::write() no mem", error_type::OS_ENOMEM);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    memset(data_sector, 0xFF, FLASH_SECTOR_SIZE);

    uint32_t ints = save_and_disable_interrupts();

    memcpy(data_sector, reinterpret_cast<const void *>(XIP_BASE + START_STORE_ADDRESS), FLASH_SECTOR_SIZE);

    memset(data_sector + get_offset(type), 0xFF, FLASH_SECTOR_SIZE / 2);

    memcpy(data_sector + get_offset(type), data, size);

    flash_range_erase(START_STORE_ADDRESS, FLASH_SECTOR_SIZE);

    for(uint i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++)
    {
        flash_range_program(START_STORE_ADDRESS + (i * FLASH_PAGE_SIZE), reinterpret_cast<const uint8_t *>(data_sector + (i * FLASH_PAGE_SIZE)), FLASH_PAGE_SIZE);
    }

    delete[] data_sector;

    restore_interrupts (ints);

	return exit::OK;
}

os::exit pico_fs_io::read(data_type type, uint8_t* data, size_t size, error** error) const OSAL_NOEXCEPT
{
    if(data == nullptr)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "pico_fs_io::read() data == null", error_type::OS_ENOENT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    if(size > FLASH_SECTOR_SIZE / 2)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "pico_fs_io::read() too much data to read", error_type::OS_ENOENT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }

    uint32_t ints = save_and_disable_interrupts();

    memcpy(data, reinterpret_cast<const void *>(XIP_BASE + START_STORE_ADDRESS + get_offset(type)), size);

    restore_interrupts (ints);

	return exit::OK;
}

os::exit pico_fs_io::clear(iface::data_type type, os::error** error) const OSAL_NOEXCEPT
{

    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase(START_STORE_ADDRESS + get_offset(type), FLASH_SECTOR_SIZE / 2);

    restore_interrupts (ints);

    return exit::OK;
}



} /* namespace driver */
} /* namespace hhg */
