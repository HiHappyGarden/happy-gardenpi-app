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

#include "hhg-driver/hardware.hpp"
#include "hhg-driver/os-config.hpp"
#include "hhg-config.h"
#include "pico/pico-uart.hpp"
#include "pico/pico-time.hpp"
#include "pico/pico-fsio.hpp"
#include "pico/pico-i2c.hpp"
#include "pico/pico-sh1106.hpp"
#include "pico/pico-rotary-encored.hpp"

//#include "ssd1306.h"


using namespace os;
using namespace hhg::iface;
using namespace hhg::driver;

namespace hhg::driver
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "HARDWARE";

}

hardware::hardware(class error** error) OS_NOEXCEPT
: time(new pico_time)
, uart(new pico_uart)
, fsio(new pico_fsio)
, i2c(new pico_i2c)
, lcd( new pico_sh1106(pico_i2c::get_i2C_reference(), 0x3C))
, rotary_encoder(new pico_rotary_encoder)
{
    if(time.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new stm32_timer) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(uart.get() == nullptr && error)
	{
        *error = OS_ERROR_BUILD("io(new pico_uart) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(fsio.get() == nullptr && error)
	{
        *error = OS_ERROR_BUILD("io(new pico_fsio) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(i2c.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new pico_i2c) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(lcd.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new lcd) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }
}


    struct test : rotary_encoder::event
    {
        test() = default;
        ~test() override = default;

        int8_t idx = 0;
        int8_t clicked = 0;

        void on_event(bool ccw, bool cw, bool click) OS_NOEXCEPT override
        {
            if(ccw)
            {
                idx--;
            }
            else if(cw)
            {
                idx++;
            }

            if(click)
            {
                clicked++;
            }

            OS_LOG_INFO(APP_TAG, "idx: %u clicked:%u", idx, clicked);
        }

    } test_one;
//    pico_ssd1306::SSD1306* display;

os::exit hardware::init(error** error) OS_NOEXCEPT
{
	if(os_config_init() == os::exit::KO)
	{
		return exit::KO;
	}

    OS_LOG_PRINTF("--------------------------\r\n");
    OS_LOG_PRINTF("%s %s\r\n", HHG_NAME, HHG_VER);
    OS_LOG_PRINTF("--------------------------\r\n");

    OS_LOG_INFO(APP_TAG, "Init Time");
    auto time_init = reinterpret_cast<hhg::iface::time_init *>(time.get());
    if(time_init == nullptr)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, " reinterpret_cast<hhg::iface::time_init *>(time.get()) fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if(time_init->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init Time - OK");

	OS_LOG_INFO(APP_TAG, "Init UART");
	if(uart->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OS_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init UART - OK");

	OS_LOG_INFO(APP_TAG, "Init FS IO");
	if(fsio->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OS_ERROR_APPEND(*error, "fsio::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init FS IO - OK");

    OS_LOG_INFO(APP_TAG, "Init I2C");
    if(i2c->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "i2c::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init I2C - OK");

    OS_LOG_INFO(APP_TAG, "Init LCD");
    if(lcd->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "lcd::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init LCD - OK");

    OS_LOG_INFO(APP_TAG, "Init rotary encoder");
    if(rotary_encoder->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init rotary rncoder - OK");

    //TODO: da rimuovere
    rotary_encoder->set_on_event(&test_one, &rotary_encoder::event::on_event);

//    for(uint8_t i = 0; i < 50; i++)
//    {
//        lcd->set_pixel(2, i, lcd::write_mode::ADD);
//    }
    lcd->set_pixel(2, 0, lcd::write_mode::ADD);
    lcd->set_pixel(2, 0, lcd::write_mode::INVERT);
    lcd->send_buffer();
    //lcd->column_remap_on();
    //lcd->invert_display();
//Create a new display object
//    display = new pico_ssd1306::SSD1306(PICO_DEFAULT_I2C_INSTANCE, 0x3C, pico_ssd1306::Size::W128xH64);
//    display->test();
//    for (int16_t y = 0; y < 64; y++){
//        display.setPixel(64, y);
//    }
//    display.clear();
//    display.sendBuffer(); //Send buffer to device and show on screen




	return exit::OK;
}

const string<128>& hardware::get_info() OS_NOEXCEPT
{
	static string<128> ret;

	return ret;
}

}
}

