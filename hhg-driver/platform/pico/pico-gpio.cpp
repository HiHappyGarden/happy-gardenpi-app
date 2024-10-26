/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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

#include "pico/pico-gpio.hpp"
#include "pico/pico-uart.hpp"
#include "pico/pico-i2c.hpp"
#include "pico/pico-rotary-encored.hpp"
#include "pico/pico-relay.hpp"
#include "pico/pico-button.hpp"
#include "pico/pico-rgb-led.hpp"
using namespace os;

#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include "osal/osal.hpp"

namespace hhg::driver
{
inline namespace v1
{

namespace
{
    void pwm_init_pin(uint8_t pin)
    {
        gpio_set_function(pin, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(pin);
        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv(&config, 4.f);
        pwm_init(slice_num, &config, true);
    }
}

os::exit init_gpio()
{

    //Set pins for uart
    gpio_set_function(pico_uart::TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(pico_uart::RX_PIN, GPIO_FUNC_UART);

    //Set pins for I2C operation
    gpio_set_function(pico_i2c::SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(pico_i2c::SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(pico_i2c::SDA_PIN);
    gpio_pull_up(pico_i2c::SCL_PIN);



    //Set pins for rotary encoder
    gpio_init(pico_rotary_encoder::ENCODER_A);
    gpio_set_dir(pico_rotary_encoder::ENCODER_A, GPIO_IN);

    gpio_init(pico_rotary_encoder::ENCODER_B);
    gpio_set_dir(pico_rotary_encoder::ENCODER_B, GPIO_IN);

    gpio_init(pico_rotary_encoder::ENCODER_BTN);
    gpio_pull_up(pico_rotary_encoder::ENCODER_BTN);
    gpio_set_dir(pico_rotary_encoder::ENCODER_BTN, GPIO_IN);

    //Set pins for relay
    gpio_init(pico_relay::RELAY_0);
    gpio_set_dir(pico_relay::RELAY_0, GPIO_OUT);

    gpio_init(pico_relay::RELAY_1);
    gpio_set_dir(pico_relay::RELAY_1, GPIO_OUT);

    gpio_init(pico_relay::RELAY_2);
    gpio_set_dir(pico_relay::RELAY_2, GPIO_OUT);

    gpio_init(pico_relay::RELAY_3);
    gpio_set_dir(pico_relay::RELAY_3, GPIO_OUT);

    //Set pins for buttons
    gpio_init(pico_button::PIN);
    gpio_pull_up(pico_button::PIN);
    gpio_set_dir(pico_button::PIN, GPIO_IN);

    //Set pins for RGB led
    pwm_init_pin(pico_rgb_led::RED);
    pwm_init_pin(pico_rgb_led::GREEN);
    pwm_init_pin(pico_rgb_led::BLUE);

    return exit::OK;
}

}
}