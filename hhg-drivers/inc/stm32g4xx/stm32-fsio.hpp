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

#include "hhg-iface/fsio.hpp"

namespace hhg::driver
{
namespace v1
{

class stm32_fsio final : public iface::v1::fsio
{
public:



	stm32_fsio() OS_NOEXCEPT;
	virtual ~stm32_fsio();

	os::exit init(os::error** error) OS_NOEXCEPT override;

	os::exit write(iface::v1::data_type type, const uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;

	os::exit read(iface::v1::data_type type, uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;
};

}
}

