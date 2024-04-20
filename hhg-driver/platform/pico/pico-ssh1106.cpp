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
using namespace os;

#include <hardware/i2c.h>

namespace hhg::driver
{
inline namespace v1
{

    namespace
    {
        constexpr char APP_TAG[] = "DRV SSH1106";
    }

    pico_ssh1106::pico_ssh1106(i2c_inst const *i2C_reference, uint16_t address, enum type type) OS_NOEXCEPT
    : i2C_reference(i2C_reference)
    , address(address)
    , type(type)
    {
        switch (type)
        {
            case type::W128xH32:
                width = 128;
                height = 32;
                break;
            case type::W128xH64:
                width = 128;
                height = 64;
                break;
        }

    }


    pico_ssh1106::~pico_ssh1106() OS_NOEXCEPT
    {
        delete[] buffer;
    }

    os::exit pico_ssh1106::init(error **error)
    {
        OS_LOG_DEBUG(APP_TAG, "Init");

        buffer_size = (width * height) / 8;
        buffer = new uint8_t [ buffer_size ];
        if(buffer == nullptr)
        {
            if(error && *error == nullptr)
            {
                *error = OS_ERROR_BUILD("buffer no mem.", error_type::OS_ENOMEM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
        }

        clear();

        // this is a list of setup commands for the display
        data setup[] =
        {
            reg_address::DISPLAY_OFF,
            reg_address::LOW_COLUMN,
            reg_address::HIGH_COLUMN,
            reg_address::START_LINE,

            reg_address::MEMORY_MODE,
            reg_address::MEMORY_MODE_HORIZONTAL,

            reg_address::CONTRAST,
            0xFF,

            reg_address::INVERTED_OFF,

            reg_address::MULTIPLEX,
            63,

            reg_address::DISPLAY_OFFSET,
            0x00,

            reg_address::DISPLAY_CLOCK_DIV,
            0x80,

            reg_address::PRE_CHARGE,
            0x22,

            reg_address::COMPINS,
            0x12,

            reg_address::VCOM_DETECT,
            0x40,

            reg_address::CHARGE_PUMP,
            0x14,

            reg_address::DISPLAY_ALL_ON_RESUME,
            reg_address::DISPLAY_ON
        };

        cmd(setup, sizeof(setup) / sizeof(setup[0]));

        send_buffer();

        return exit::OK;
    }

    void pico_ssh1106::set_pixel(int16_t x, int16_t y, lcd::write_mode mode) const
    {

        if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
            return;


        buffer[y] = 1;

        switch (mode)
        {
            case lcd::write_mode::ADD:

                break;
            case lcd::write_mode::SUBTRACT:

                break;
            case lcd::write_mode::INVERT:

                break;
        }

    }


    void pico_ssh1106::send_buffer()
    {
        cmd(reg_address::PAGE_ADDR); //Set page address from min to max
        cmd(0x00);
        cmd(0x07);
        cmd(reg_address::COLUMN_ADDR); //Set column address from min to max
        cmd(0x00);
        cmd(127);

        // create a temporary buffer of size of buffer plus 1 byte for startline command aka 0x40
        auto data = new uint8_t [buffer_size + 1];
        if(data == nullptr)
        {
            OS_LOG_FATAL(APP_TAG, "Impossible alloc data");
            return;
        }

        data[0] = static_cast<uint8_t>(reg_address::START_LINE);

        // copy framebuffer to temporary buffer
        memcpy(data + 1, buffer, buffer_size);

        // send data to device
        i2c_write_blocking(const_cast<i2c_inst_t *>(i2C_reference), address, data, buffer_size + 1, false);

        delete[] data;
    }

    void pico_ssh1106::add_bitmap_image(int16_t anchor_x, int16_t anchor_y, uint8_t image_width, uint8_t image_height,uint8_t *image, iface::lcd::write_mode mode)
    {

    }

    void pico_ssh1106::set_buffer(uint8_t *buffer)
    {
        memcpy(this->buffer, buffer, buffer_size);
    }

    void pico_ssh1106::set_orientation(bool orientation)
    {

    }

    void pico_ssh1106::clear()
    {
        memset(buffer, 0, buffer_size);
    }

    void pico_ssh1106::invert_display()
    {

    }

    void pico_ssh1106::set_contrast(uint8_t contrast)
    {

    }

    void pico_ssh1106::turn_off() const
    {

    }

    void pico_ssh1106::turn_on() const
    {

    }

    void pico_ssh1106::cmd(uint8_t command) const
    {
        uint8_t data[2] = {0x00, command};
        i2c_write_blocking(const_cast<i2c_inst *>(this->i2C_reference), this->address, data, 2, false);
    }

    void pico_ssh1106::cmd(const data commands[], uint8_t data_len) const
    {
        if(commands == nullptr)
        {
            return;
        }
        for(uint8_t i = 0; i < data_len; i++)
        {
            cmd(commands[i].value());
        }
    }
}
}