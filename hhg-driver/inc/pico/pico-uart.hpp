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

#include "hhg-iface/io.hpp"

#include "pico/types.h"

namespace hhg::driver
{
inline namespace v1
{


class pico_uart final : public hhg::iface::io_initializable
{
	const hhg::iface::io::receive* obj = nullptr;
	on_receive on_receive_callback = nullptr;

	static inline pico_uart* singleton = nullptr;

    public:

    enum pin : uint
    {
        TX_PIN = 0,
        RX_PIN = 1
    };

    pico_uart() OSAL_NOEXCEPT;
	~pico_uart() OSAL_NOEXCEPT override;
	OSAL_NO_COPY_NO_MOVE(pico_uart)

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

	void set_on_receive(const hhg::iface::io::receive* obj, on_receive on_receive_callback) OSAL_NOEXCEPT override;

    os::exit transmit(const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT override;

};

}
}
