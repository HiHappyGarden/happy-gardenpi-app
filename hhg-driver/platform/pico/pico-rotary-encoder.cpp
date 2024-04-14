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

//ref https://www.reddit.com/r/raspberrypipico/comments/pacarb/sharing_some_c_code_to_read_a_rotary_encoder/

#include "pico/pico-rotary-encored.hpp"
using namespace os;

#include <hardware/gpio.h>

namespace hhg::driver
{
inline namespace v1
{

    pico_rotary_encoder::pico_rotary_encoder() OS_NOEXCEPT = default;

    pico_rotary_encoder::~pico_rotary_encoder() OS_NOEXCEPT = default;


    os::exit pico_rotary_encoder::init(os::error **error) OS_NOEXCEPT
    {
        if(singleton)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_rotary_encoder::init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
        singleton = this;


        return exit::OK;
    }

    void pico_rotary_encoder::encoder_callback(uint gpio, uint32_t events)
    {
        uint32_t gpio_state = 0;

        uint i = gpio_get_all();

        gpio_state = (gpio_get_all() >> 19) & 0b0111;  	// get all GPIO them mask out all but bits 10, 11, 12
        // This will need to change to match which GPIO pins are being used.


        static bool ccw_fall = false;  //bool used when falling edge is triggered
        static bool cw_fall = false;

        uint8_t enc_value = 0;
        enc_value = (gpio_state & 0x03);


        if (gpio == hhg::driver::pico_rotary_encoder::ENCODER_A)
        {

            if ((!cw_fall) && (enc_value == 0b10)) // cw_fall is set to TRUE when phase A interrupt is triggered
                cw_fall = true;

            if ((ccw_fall) && (enc_value == 0b00)) // if ccw_fall is already set to true from a previous B phase trigger, the ccw event will be triggered
            {
                cw_fall = false;
                ccw_fall = false;
                //do something here,  for now it is just printing out CW or CCW
                printf("CCW \r\n");
            }

        }

        if (gpio == hhg::driver::pico_rotary_encoder::ENCODER_B)
        {
            if ((!ccw_fall) && (enc_value == 0b01)) //ccw leading edge is true
                ccw_fall = true;

            if ((cw_fall) && (enc_value == 0b00)) //cw trigger
            {
                cw_fall = false;
                ccw_fall = false;
                //do something here,  for now it is just printing out CW or CCW
                printf("CW \r\n");
            }

        }

        if (gpio == hhg::driver::pico_rotary_encoder::ENCODER_BTN)
        {
            printf("CLICK \r\n");
        }
    }
}
}

