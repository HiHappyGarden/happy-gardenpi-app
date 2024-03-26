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




#define DATA_32                     ((uint32_t)0x12345678)
#define DATA_64                     ((uint64_t)0x5f5f6369616f5f5f)


namespace hhg::driver
{
namespace v1
{

uint64_t const pico_fsio::check_data = 0x0102030404030201;

pico_fsio::pico_fsio(uint32_t start_flash_address, uint32_t end_flash_address) OS_NOEXCEPT
: start_flash_address(start_flash_address)
, end_flash_address(end_flash_address)
{

}

pico_fsio::~pico_fsio()
{

}

os::exit pico_fsio::init(error** error) OS_NOEXCEPT
{


	return exit::OK;
}

os::exit pico_fsio::write(data_type type, const uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

	return exit::OK;
}

os::exit pico_fsio::read(data_type type, uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

	return exit::OK;
}




} /* namespace driver */
} /* namespace hhg */
