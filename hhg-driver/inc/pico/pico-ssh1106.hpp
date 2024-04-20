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


#pragma once

#include "osal/osal.hpp"
#include "hhg-iface/lcd.hpp"
#include "hhg-iface/initializable.hpp"

#include <pico/types.h>

struct i2c_inst;
typedef struct i2c_inst i2c_inst_t;


namespace hhg::driver
{

inline namespace v1
{



    class pico_ssh1106 : public hhg::iface::lcd
{
public:
    /// \enum pico_ssd1306::Size
    enum class type {
        /// Display size W128xH64
        W128xH64,
        /// Display size W128xH32
        W128xH32
    };

    enum class reg_address : uint8_t
    {
        CONTRAST = 0x81,
        DISPLAY_ALL_ON_RESUME = 0xA4,
        DISPLAY_ALL_ON = 0xA5,
        INVERTED_OFF = 0xA6,
        INVERTED_ON = 0xA7,
        DISPLAY_OFF = 0xAE,
        DISPLAY_ON = 0xAF,
        DISPLAY_OFFSET = 0xD3,
        COMPINS = 0xDA,
        VCOM_DETECT = 0xDB,
        DISPLAY_CLOCK_DIV = 0xD5,
        PRE_CHARGE = 0xD9,
        MULTIPLEX = 0xA8,
        LOW_COLUMN = 0x00,
        HIGH_COLUMN = 0x10,
        START_LINE = 0x40,
        MEMORY_MODE = 0x20,
        MEMORY_MODE_HORIZONTAL = 0x00,
        MEMORY_MODE_VERTICAL = 0x01,
        MEMORY_MODE_PAGE = 0x10,
        COLUMN_ADDR = 0x21,
        PAGE_ADDR = 0x22,
        COM_REMAP_OFF = 0xC0,
        COM_REMAP_ON = 0xC8,
        COLUMN_REMAP_OFF = 0xA0,
        COLUMN_REMAP_ON = 0xA1,
        CHARGE_PUMP = 0x8D,
        EXTERNAL_VCC = 0x01,
        SWIT_CHCAP_VCC = 0x02,
    };

    class data
    {
        uint8_t const b;
    public:
        constexpr inline data(uint8_t byte) : b(byte) OS_NOEXCEPT {} // no explicit
        constexpr inline data(reg_address byte) : b(static_cast<uint8_t>(byte)) OS_NOEXCEPT {} // no explicit
        constexpr inline uint8_t value() const  OS_NOEXCEPT { return b; }
    };


    pico_ssh1106(i2c_inst const *i2C_reference, uint16_t address, enum type type) OS_NOEXCEPT;
    ~pico_ssh1106() OS_NOEXCEPT override;
    OS_NO_COPY_NO_MOVE(pico_ssh1106)

    os::exit init(os::error **error) OS_NOEXCEPT override;

    void set_pixel(int16_t x, int16_t y, write_mode mode) const OS_NOEXCEPT override;

    void send_buffer() OS_NOEXCEPT override;

    void add_bitmap_image(int16_t anchor_x, int16_t anchor_y, uint8_t image_width, uint8_t image_height, uint8_t *image, write_mode mode) OS_NOEXCEPT override;

    void set_buffer(uint8_t *buffer) OS_NOEXCEPT override;

    void set_orientation(bool orientation) OS_NOEXCEPT override;

    void clear() OS_NOEXCEPT override;

    void invert_display() OS_NOEXCEPT override;

    void set_contrast(uint8_t contrast) OS_NOEXCEPT override;

    void turn_off() const OS_NOEXCEPT override;

    void turn_on() const OS_NOEXCEPT override;

private:
    i2c_inst const *i2C_reference = nullptr;
    uint16_t const address = 0;
    type const type = type::W128xH64;

    uint8_t width = 0;
    uint8_t height = 0;

    size_t buffer_size = 0;
    uint8_t* buffer = nullptr;

    void cmd(uint8_t command) OS_NOEXCEPT const override;

    inline void cmd(const data& addr) const OS_NOEXCEPT
    {
        cmd(addr.value());
    }

    void cmd(const data* commands, uint8_t data_len) const OS_NOEXCEPT;


    };

}
}

