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

#include "hhg-driver/hardware.hpp"
#include "hhg-driver/os-config.hpp"
#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif
#include "pico/pico-uart.hpp"
#include "pico/pico-time.hpp"
#include "pico/pico-fs-io.hpp"
#include "pico/pico-i2c.hpp"
#include "pico/pico-relay.hpp"
#include "pico/pico-sh1106.hpp"
#include "pico/pico-rotary-encored.hpp"
#include "pico/pico-button.hpp"
#include "pico/pico-rgb-led.hpp"
#include "pico/pico-wifi.hpp"
#include "pico/pico-mqtt.hpp"

#include <pico/unique_id.h>

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



hardware::hardware(class error** error) OSAL_NOEXCEPT
: time(new pico_time)
, uart(new pico_uart)
, fs_io(new pico_fs_io)
, i2c(new pico_i2c)
, relay(new pico_relay)
, lcd( new pico_sh1106(i2c->get_i2C_reference(), pico_sh1106::ADDRESS))
, rotary_encoder(new pico_rotary_encoder)
, button(new pico_button)
, rgb_led(new pico_rgb_led)
, mqtt(new pico_mqtt(error))
, wifi(new pico_wifi(mqtt))
{
    if(time.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("time(new stm32_timer) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(uart.get() == nullptr && error)
	{
        *error = OSAL_ERROR_BUILD("uart(new pico_uart) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(fs_io.get() == nullptr && error)
	{
        *error = OSAL_ERROR_BUILD("fs_io(new pico_fs_io) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(i2c.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("i2c(new pico_i2c) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(relay.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("relay(new pico_relay) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(lcd.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("lcd(new pico_lcd) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(rotary_encoder.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("rotary_encoder(new pico_rotary_encoder) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(button.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("button(new pico_button) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(rgb_led.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("rgb_led(new pico_rgb_led) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(wifi.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("wifi(new pico_wifi) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(mqtt.get() == nullptr && error)
    {
        *error = OSAL_ERROR_BUILD("mqtt(new pico_mqtt) no mem.", error_type::OS_ENOMEM);
        OSAL_ERROR_PTR_SET_POSITION(*error);
        return;
    }
}



os::exit hardware::init(error** error) OSAL_NOEXCEPT
{
	if(os_config_init() == os::exit::KO)
	{
		return exit::KO;
	}

    OSAL_LOG_PRINTF("-------------------------------------------\r\n");
    OSAL_LOG_PRINTF("%s %s\r\n", HHG_NAME, get_info().c_str());
    OSAL_LOG_PRINTF("-------------------------------------------\r\n");

    OSAL_LOG_INFO(APP_TAG, "Init RGB led");
    if(rgb_led->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "rgb_led::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init RGB led - OK");
    rgb_led->set_rgb(0xFF, 0xA5, 0x00);

    OSAL_LOG_INFO(APP_TAG, "Init Time");
    auto time_init = reinterpret_cast<hhg::iface::time_init *>(time.get());
    if(time_init == nullptr)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, " reinterpret_cast<hhg::iface::time_init *>(time.get()) fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if(time_init->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init Time - OK");

	OSAL_LOG_INFO(APP_TAG, "Init UART");
	if(uart->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OSAL_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init UART - OK");

	OSAL_LOG_INFO(APP_TAG, "Init FS IO");
	if(fs_io->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OSAL_ERROR_APPEND(*error, "fs_io::init() fail.", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OSAL_LOG_INFO(APP_TAG, "Init FS IO - OK");

    OSAL_LOG_INFO(APP_TAG, "Init I2C");
    if(i2c->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "i2c::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init I2C - OK");

    OSAL_LOG_INFO(APP_TAG, "Init relay");
    if(relay->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "relay::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init relay - OK");

    OSAL_LOG_INFO(APP_TAG, "Init LCD");
    if(lcd->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "lcd::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init LCD - OK");

    OSAL_LOG_INFO(APP_TAG, "Init rotary encoder");
    if(rotary_encoder->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init rotary encoder - OK");

    OSAL_LOG_INFO(APP_TAG, "Init button");
    if(button->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "button::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init button - OK");

    OSAL_LOG_INFO(APP_TAG, "Init WiFI");
    if(wifi->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OSAL_ERROR_APPEND(*error, "wifi::init() fail.", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OSAL_LOG_INFO(APP_TAG, "Init WiFI - OK");
    //mqtt->init(nullptr);
	return exit::OK;
}

    const os::string<16> &hardware::get_serial() OSAL_NOEXCEPT
    {
        static string<16> ret;
        pico_unique_board_id_t board_id;
        pico_get_unique_board_id(&board_id);

        for (uint8_t i : board_id.id)
        {
            char buff[] = {'\0','\0','\0'};
            snprintf(buff, sizeof(buff), "%02x", board_id.id[i]);
            ret += buff;
        }

        return ret;
    }


const string<128>& hardware::get_info() OSAL_NOEXCEPT
{
	static string<128> ret;

    ret += HHG_VER;
    ret += "|";
    ret += get_serial().c_str();

	return ret;
}


}
}

