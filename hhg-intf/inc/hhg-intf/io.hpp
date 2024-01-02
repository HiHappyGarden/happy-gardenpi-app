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

#include "osal/osal.hpp"
#include "hhg-intf/initializable.hpp"

namespace hhg::intf
{
inline namespace v1
{

class io : public initializable
{
public:
	using ptr = os::unique_ptr<hhg::intf::io>;

	using on_read = void (*)(const uint8_t data[], uint16_t size);

    virtual ~io() = default;

    virtual void set_on_read(on_read) OS_NOEXCEPT = 0;

    virtual os::exit write(const uint8_t data[], uint16_t size) const OS_NOEXCEPT = 0;
};

}
}
