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

#include "pico/pico-sh1106.hpp"
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

    pico_sh1106::pico_sh1106(i2c_inst const *i2c_reference, uint16_t address) OS_NOEXCEPT
    : i2c_reference(i2c_reference)
    , address(address)
    {

    }


    pico_sh1106::~pico_sh1106() OS_NOEXCEPT = default;

    os::exit pico_sh1106::init(class error **error) OS_NOEXCEPT
    {
        OS_LOG_DEBUG(APP_TAG, "Init");

        if(buffer == nullptr)
        {
            if(error && *error == nullptr)
            {
                *error = OS_ERROR_BUILD("buffer no mem.", error_type::OS_ENOMEM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
        }

        this->error = error;

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
                0x3f,

                reg_address::DISPLAY_OFFSET,
                0x00,

                reg_address::DISPLAY_CLOCK_DIV,
                0x80,

                reg_address::PRE_CHARGE,
                0xF1,

                reg_address::COM_PADS,
                0x12,

                reg_address::VCOM_DETECT,
                0x40,

                reg_address::CHARGE_PUMP,
                0x14,

                reg_address::DISPLAY_ALL_ON_RESUME,
                reg_address::DISPLAY_ON
        };

        send_cmd(setup, sizeof(setup) / sizeof(setup[0]));

        clear();
        send_buffer();

        return exit::OK;
    }

    void pico_sh1106::set_pixel(uint16_t x, uint16_t y, lcd::write_mode mode) const
    {
        if ((x < 0) || (x >= width) || (y < 0) || (y >= (height * 8) ))
            return;

        uint8_t page = y / 8;
        uint8_t bit = y % 8;
        uint16_t idx = (page * width) + x;

        switch (mode)
        {
            case lcd::write_mode::ADD:
                buffer[idx] |= (1 << bit);
                break;
            case lcd::write_mode::REMOVE:
                buffer[idx] &= ~(1 << bit);
                break;
            case lcd::write_mode::INVERT:
                buffer[idx] ^= (1 << bit);
                break;
        }

    }


    void pico_sh1106::send_buffer() OS_NOEXCEPT
    {
        send_cmd(reg_address::LOW_COLUMN);
        send_cmd(reg_address::HIGH_COLUMN);
        for(uint8_t i = 0; i < height; i++)
        {
            send_cmd(reg_address::PAGE_ADDR, i);
            send_row(buffer + (i * width));
        }
    }

    void pico_sh1106::set_bitmap_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image, uint32_t image_size) OS_NOEXCEPT
    {
        if(image == nullptr || width * height != image_size)
        {
            return;
        }

        uint32_t idx = 0;


        for(uint16_t h = 0; h < height; h++)
        {
            for(uint16_t w = 0; w < width; w++)
            {
                if(image[idx])
                {
                    set_pixel(x + w, y + h, write_mode::ADD);
                }
                else
                {
                    set_pixel(x + w, y + h, write_mode::REMOVE);
                }
                idx++;
            }
        }
    }

    void pico_sh1106::set_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, write_mode mode) OS_NOEXCEPT
    {
        for(uint16_t w = 0; w < width; w++)
        {
            for(uint16_t h = 0; h < height; h++)
            {
                set_pixel(x + w, y + h, mode);
            }
        }
    }

    void pico_sh1106::set_char(char c, uint16_t x, uint16_t y, const uint8_t* font, uint32_t font_size) OS_NOEXCEPT
    {
        if(font == nullptr)
        {
            return;
        }

        uint8_t width = font[0];
        uint8_t height = (font[1] / 8) + (font[1] % 8 ? 1 : 0);
        uint8_t single_font_size = width * height;

        if((font_size - 2) % width != 0)
        {
            OS_LOG_ERROR(APP_TAG, "The table font it's odd");
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_sh1106::set_char() the table font it's odd", error_type::OS_ECOMM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return;
        }

        uint32_t c_offset = ( (c - 32) * (width * height) ) + 2;


        uint32_t i = 80;

        uint8_t w = 0;
        uint8_t h = 0;
        for(uint32_t idx = 0; idx < single_font_size; idx++)
        {
            if(w >= width)
            {
                h++;
                w = 0;
            }

            if(h == height)
            {
                break;
            }

            if(w < width)
            {
                for(uint8_t bit = 0; bit < 8; bit++)
                {
                    OS_LOG_ERROR(APP_TAG, "idx = %d, x + w = %u, y + h + bit = %u", idx, x + w, y + (h * 8) + bit);
                    if (font[c_offset + idx] & (1 << bit))
                    {
                        set_pixel(x + w, y + (h * 8) + bit, write_mode::ADD);
                    }
                    else
                    {
                        set_pixel(x + w, y + (h * 8) + bit, write_mode::REMOVE);
                    }
                }
                w++;
            }

        }
    }

    void pico_sh1106::set_str(const char *str, uint16_t x, uint16_t y, const uint8_t *font, uint32_t font_size) OS_NOEXCEPT
    {

    }

    void pico_sh1106::set_buffer(uint8_t *buffer, size_t buffer_size) OS_NOEXCEPT
    {
        if(buffer_size < this->buffer_size)
        {
            memcpy(this->buffer, buffer, buffer_size);
            memset(this->buffer, 0, this->buffer_size - buffer_size);
        }
        else if(buffer_size > this->buffer_size)
        {
            memcpy(this->buffer, buffer, this->buffer_size);
        }
        else
        {
            memcpy(pico_sh1106::buffer, buffer, pico_sh1106::buffer_size);
        }
    }

    void pico_sh1106::invert_orientation() OS_NOEXCEPT
    {
        this->orientation = !orientation;
        if(this->orientation)
        {
            send_cmd(reg_address::COLUMN_REMAP_OFF);
            send_cmd(reg_address::VERTICAL_FLIP_OFF);
        }
        else
        {
            send_cmd(reg_address::COLUMN_REMAP_ON);
            send_cmd(reg_address::VERTICAL_FLIP_ON);
        }
    }

    void pico_sh1106::clear() OS_NOEXCEPT
    {
        memset(buffer, 0b00000000, buffer_size);
    }

    void pico_sh1106::set_contrast(uint8_t contrast) OS_NOEXCEPT
    {
        data data[] = {reg_address::SET_CONTRAST, contrast};
        send_cmd(data, sizeof(data));
    }

    void pico_sh1106::turn_off() const OS_NOEXCEPT
    {
        send_cmd(reg_address::DISPLAY_OFF);
    }

    void pico_sh1106::turn_on() const OS_NOEXCEPT
    {
        send_cmd(reg_address::DISPLAY_ON);
    }


    void pico_sh1106::send_cmd(uint8_t command) const OS_NOEXCEPT
    {
        uint8_t data[2] = {0x00, command};
        if(i2c_write_blocking(const_cast<i2c_inst *>(i2c_reference), this->address, data, sizeof(data), false) != sizeof(data))
        {
            OS_LOG_ERROR(APP_TAG, " pico_sh1106::send_cmd() send cmd error");
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_sh1106::send_cmd() send cmd error", error_type::OS_ECOMM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
        }
    }

    void pico_sh1106::send_cmd(const data commands[], uint8_t data_len) const OS_NOEXCEPT
    {
        if(commands == nullptr)
        {
            return;
        }
        for(uint8_t i = 0; i < data_len; i++)
        {
            send_cmd(commands[i].value());
        }
    }

    void pico_sh1106::send_data(const uint8_t* buff, size_t buff_size, uint8_t offset) const OS_NOEXCEPT
    {
        buff_size++;

        auto data = new uint8_t[buff_size];
        data[0] = static_cast<uint8_t>(reg_address::START_LINE);

        memcpy(data + 1, buff, buff_size - 1);

        if(i2c_write_blocking(const_cast<i2c_inst *>(i2c_reference), address, data, buff_size, false) != buff_size)
        {
            OS_LOG_ERROR(APP_TAG, "pico_sh1106::send_data() send data error");
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_sh1106::send_data() send data error", error_type::OS_ECOMM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
        }

        delete[] data;
    }

    void pico_sh1106::send_row(const uint8_t* buff) const OS_NOEXCEPT
    {
        uint8_t data[width + 1];
        data[0] = static_cast<uint8_t>(reg_address::START_LINE);

        memcpy(data + 1, buff, sizeof(data)- 1);

        if(i2c_write_blocking(const_cast<i2c_inst *>(i2c_reference), address, data, sizeof(data), false) != sizeof(data))
        {
            OS_LOG_ERROR(APP_TAG, "pico_sh1106::send_row() send row error");
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_sh1106::send_row() send row error", error_type::OS_ECOMM);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
        }
    }

}
}