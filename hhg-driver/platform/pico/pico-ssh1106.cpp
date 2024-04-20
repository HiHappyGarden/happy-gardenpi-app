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

#include "pico/pico-ssh1106.hpp"


namespace hhg::driver
{
inline namespace v1
{

    pico_ssh1106::pico_ssh1106() = default;
    pico_ssh1106::~pico_ssh1106() OS_NOEXCEPT = default;

    void pico_ssh1106::set_pixel(int16_t x, int16_t y, iface::lcd::WriteMode mode) {

    }

    void pico_ssh1106::send_buffer() {

    }

    void pico_ssh1106::add_bitmap_image(int16_t anchor_x, int16_t anchor_y, uint8_t image_width, uint8_t image_height,
                                        uint8_t *image, iface::lcd::WriteMode mode) {

    }

    void pico_ssh1106::set_buffer(unsigned char *buffer) {

    }

    void pico_ssh1106::set_orientation(bool orientation) {

    }

    void pico_ssh1106::clear() {

    }

    void pico_ssh1106::invert_display() {

    }

    void pico_ssh1106::set_contrast(unsigned char contrast) {

    }

    void pico_ssh1106::turn_off() const {

    }

    void pico_ssh1106::turn_on() const {

    }
}
}