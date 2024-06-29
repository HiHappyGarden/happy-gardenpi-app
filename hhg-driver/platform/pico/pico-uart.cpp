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

#include "pico/pico-uart.hpp"
using hhg::iface::io;
using hhg::iface::io_source;
using namespace os;

#include <pico/stdlib.h>
#include <hardware/uart.h>




namespace hhg::driver
{
inline namespace v1
{

namespace
{
constexpr uint32_t BAUD_RATE = 115200;
constexpr uint8_t DATA_BITS = 8;
constexpr uint8_t STOP_BITS = 1;
}

pico_uart::pico_uart() OSAL_NOEXCEPT = default;

pico_uart::~pico_uart()
{
	singleton = nullptr;
}

os::exit pico_uart::init(error** error) OSAL_NOEXCEPT
{
	if(singleton)
	{
		if(error)
		{
	        *error = OSAL_ERROR_BUILD("pico_uart::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	singleton = this;

    uart_init(uart0, BAUD_RATE);

    irq_set_exclusive_handler(UART0_IRQ, []
    {

        while (uart_is_readable(uart0))
        {
            uint8_t ch = uart_getc(uart0);
            if(pico_uart::singleton && pico_uart::singleton->obj && pico_uart::singleton->on_receive_callback)
            {
                (pico_uart::singleton->obj->*pico_uart::singleton->on_receive_callback)(io_source::UART, &ch, 1);
            }
        }

    });

    irq_set_enabled(UART0_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(uart0, true, false);


	return exit::OK;
}


inline void pico_uart::set_on_receive(const receive* obj, on_receive on_receive_callback) OSAL_NOEXCEPT
{
	this->obj = obj;
	this->on_receive_callback = on_receive_callback;
}

os::exit pico_uart::transmit(const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT
{
    for(uint16_t i = 0; i < size; i++)
    {
        uart_putc(uart0, data[i]);
    }

	return exit::OK;
}

}
}
