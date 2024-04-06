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

#include "hhg-driver/lcd.hpp"

namespace hhg::driver
{
inline namespace v1
{

struct pico_lcd_io final : public lcd::io
{
    const uint8_t addr = 0x27;

    ~pico_lcd_io() override = default;
    os::exit init(os::error** error) const OS_NOEXCEPT override;
    int32_t write(const uint8_t* data, size_t data_len) const OS_NOEXCEPT override;
    int32_t read(uint8_t* data, size_t data_size) const OS_NOEXCEPT override;
    void us_sleep(uint64_t us) const OS_NOEXCEPT override;
};


class pico_lcd final : public lcd
{
    const pico_lcd_io io;
public:
    inline pico_lcd(uint8_t cols, uint8_t rows) OS_NOEXCEPT
    : lcd(io, cols, rows)
    {

    }
    ~pico_lcd() override = default;
};

}
}
