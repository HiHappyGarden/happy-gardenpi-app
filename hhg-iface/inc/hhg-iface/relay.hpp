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

namespace hhg::iface
{
inline namespace v1
{

struct relay : public initializable
{
    using ptr = os::unique_ptr<hhg::iface::relay>;

    ~relay() OS_NOEXCEPT override = default;

    virtual uint8_t size() const OS_NOEXCEPT = 0;

    virtual bool operator [](uint8_t idx) OS_NOEXCEPT = 0;

    virtual bool operator [](uint8_t idx) const OS_NOEXCEPT = 0;

    virtual bool set(uint8_t pin, bool value) OS_NOEXCEPT = 0;
};


}
}