/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-iface/initializable.hpp"

namespace hhg::iface
{
inline namespace v1
{

enum class io_source
{
	UART,
	WIFI,
    DISPLAY
};


struct io
{

    struct receive
    {
        virtual ~receive() = default;

        virtual void on_receive(io_source io_source, const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT = 0;
    };

	using ptr = os::unique_ptr<hhg::iface::io>;

	using on_receive = void (receive::*)(io_source io_source, const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT;

    virtual ~io() = default;

    virtual void set_on_receive(const receive*, on_receive) OSAL_NOEXCEPT = 0;

    virtual os::exit transmit(const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT = 0;
};

struct io_initializable : public hhg::iface::io, public hhg::iface::initializable
{
    using ptr = os::unique_ptr<hhg::iface::io_initializable>;

    ~io_initializable() override = default;
};



}
}
