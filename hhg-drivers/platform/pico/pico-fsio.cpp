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


#include "pico/pico-fsio.hpp"
using namespace hhg::iface;
using namespace os;

#include <hardware/sync.h>
#include <hardware/flash.h>

namespace hhg::driver
{
namespace v1
{

uint64_t const pico_fsio::check_data = 0x0102030404030201;

pico_fsio::pico_fsio() = default;

pico_fsio::~pico_fsio() = default;

os::exit pico_fsio::init(error** error) OS_NOEXCEPT
{


	return exit::OK;
}

os::exit pico_fsio::write(data_type type, const uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{
    char old_data[FLASH_SECTOR_SIZE];

    memcpy(old_data, reinterpret_cast<const void *>(start_flash_address), FLASH_SECTOR_SIZE);

    uint32_t ints = save_and_disable_interrupts();

    flash_range_erase(start_flash_address, FLASH_SECTOR_SIZE);

    //flash_range_program(start_flash_address, data, FLASH_PAGE_SIZE);

    restore_interrupts (ints);

	return exit::OK;
}

os::exit pico_fsio::read(data_type type, uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

	return exit::OK;
}

os::exit pico_fsio::clear(iface::data_type type, os::error** error) const OS_NOEXCEPT
{
    return exit::OK;
}



} /* namespace driver */
} /* namespace hhg */
