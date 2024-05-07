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

#include "hhg-config.h"
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


class pico_sh1106 : public hhg::iface::lcd
{
public:

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
        COM_PADS = 0xDA,
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
        PAGE_ADDR = 0xB0,
        VERTICAL_FLIP_OFF = 0xC0,
        VERTICAL_FLIP_ON = 0xC8,
        COLUMN_REMAP_OFF = 0xA0,
        COLUMN_REMAP_ON = 0xA1,
        CHARGE_PUMP = 0x8D,
        EXTERNAL_VCC = 0x1,
        SWITCH_CAP_VCC = 0x2,
        SET_CONTRAST = 0x81
    };

    class data final
    {
        uint8_t const b;
    public:
        constexpr inline data(uint8_t byte) : b(byte) OS_NOEXCEPT {} // no explicit
        constexpr inline data(reg_address byte) : b(static_cast<uint8_t>(byte)) OS_NOEXCEPT {} // no explicit
        constexpr inline uint8_t value() const  OS_NOEXCEPT { return b; }
    };

    pico_sh1106(i2c_inst const *i2c_reference, uint16_t address) OS_NOEXCEPT;
    ~pico_sh1106() OS_NOEXCEPT override;
    OS_NO_COPY_NO_MOVE(pico_sh1106)

    os::exit init(class os::error **error) OS_NOEXCEPT override;

    void set_pixel(uint8_t x, uint8_t y, write_mode mode) const OS_NOEXCEPT override;

    inline void set_pixel(uint8_t x, uint8_t y) const OS_NOEXCEPT override
    {
        set_pixel(x, y, write_mode::ADD);
    }

    void send_buffer() OS_NOEXCEPT override;

    void set_bitmap_image(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t *image, uint32_t image_size) OS_NOEXCEPT override;

    void set_rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, write_mode mode) OS_NOEXCEPT override;

    void set_char(uint8_t c, uint8_t x, uint8_t y, const uint8_t* font, uint32_t font_size) OS_NOEXCEPT override;

    void set_buffer(uint8_t *buffer, size_t buffer_size) OS_NOEXCEPT override;

    void invert_orientation() OS_NOEXCEPT override;

    void clear() OS_NOEXCEPT override;

    void set_contrast(uint8_t contrast) OS_NOEXCEPT override;

    void turn_off() const OS_NOEXCEPT override;

    void turn_on() const OS_NOEXCEPT override;

private:
    i2c_inst const *i2c_reference = nullptr;
    uint16_t const address = 0;

    static constexpr uint8_t width = 132;
    static constexpr uint8_t height = 8;

    static constexpr size_t buffer_size = width * height;
    mutable uint8_t buffer[buffer_size];

    bool orientation = true;

    os::error** error = nullptr;

    void send_cmd(uint8_t command) OS_NOEXCEPT const;

    inline void send_cmd(const data& addr, uint8_t or_data = 0x00) const OS_NOEXCEPT
    {
        send_cmd(addr.value() | or_data);
    }

    void send_cmd(const data* commands, uint8_t data_len) const OS_NOEXCEPT;

    void send_data(const uint8_t* buff, size_t buff_size, uint8_t offset) OS_NOEXCEPT const;

    void send_row(const uint8_t* buff) const OS_NOEXCEPT;

};

}
}

