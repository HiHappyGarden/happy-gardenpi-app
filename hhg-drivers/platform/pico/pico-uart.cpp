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

#include <pico/pico-uart.hpp>
using hhg::iface::io_on_receive;
using hhg::iface::io_source;

#include "stm32g4xx/driver-lpuart.h"
using namespace os;

#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart0

namespace hhg::driver
{
inline namespace v1
{


pico_uart::pico_uart() OS_NOEXCEPT = default;

pico_uart::~pico_uart()
{
	singleton = nullptr;
}

os::exit pico_uart::init(error** error) OS_NOEXCEPT
{
	if(singleton)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("pico_uart::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	singleton = this;

    //    // Use some the various UART functions to send out data
//    // In a default system, printf will also output via the default UART
//
//    // Send out a character without any conversions
//    uart_putc_raw(UART_ID, 'A');
//
//    // Send out a character but do CR/LF conversions
//    uart_putc(UART_ID, 'B');
//
//    // Send out a string, with CR/LF conversions
//    uart_puts(UART_ID, " Hello, UART!\n");

    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    irq_set_exclusive_handler(UART_IRQ, []
    {

        while (uart_is_readable(UART_ID))
        {
            uint8_t ch = uart_getc(UART_ID);
            if(pico_uart::singleton && pico_uart::singleton->obj && pico_uart::singleton->on_receive_callback)
            {
                (pico_uart::singleton->obj->*pico_uart::singleton->on_receive_callback)(io_source::UART, &ch, 1);
            }
        }

    });
    irq_set_enabled(UART_IRQ, true);

	return exit::OK;
}


inline void pico_uart::set_on_receive(const io_on_receive* obj, on_receive on_receive_callback) OS_NOEXCEPT
{
	this->obj = obj;
	this->on_receive_callback = on_receive_callback;
}

os::exit pico_uart::transmit(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{
    for(uint16_t i = 0; i < size; i++)
    {
        uart_putc(UART_ID, data[i]);
    }

	return exit::OK;
}

}
}
