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


#include "stm32g4xx/stm32-fsio.hpp"
using namespace hhg::iface;
using namespace os;

namespace hhg::driver
{
namespace v1
{

stm32_fsio::stm32_fsio() OS_NOEXCEPT
{

}

stm32_fsio::~stm32_fsio() = default;

os::exit stm32_fsio::init(error** error) OS_NOEXCEPT
{

	return exit::OK;
}

os::exit stm32_fsio::write(data_type type, const uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

	return exit::OK;
}

os::exit stm32_fsio::read(data_type type, uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

	return exit::OK;
}

} /* namespace driver */
} /* namespace hhg */
