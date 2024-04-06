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

#include "pico/pico-lcd.hpp"
#include "pico/pico-i2c.hpp"
using namespace os;

namespace hhg::driver {
inline namespace v1 {

os::exit pico_lcd_io::init(os::error **error) const OS_NOEXCEPT
{
    uint8_t rx_data;
    return i2c_read_blocking(i2c_default, addr, &rx_data, 1, false) != 0 ? exit::OK : exit::KO;
}


int32_t pico_lcd_io::write(const uint8_t *data, size_t data_len) const OS_NOEXCEPT
{
    auto ret = i2c_write_timeout_us(i2c_default, addr, data, data_len, false, 1'000'000);
    if(ret == PICO_ERROR_GENERIC)
    {
        return -static_cast<int32_t>(error_type::OS_EIO);
    }
    return 0;
}

int32_t pico_lcd_io::read(uint8_t *data, size_t data_size) const OS_NOEXCEPT
{
    auto ret = i2c_read_timeout_us(i2c_default, addr, data, data_size, false, 1'000'000);
    if(ret == PICO_ERROR_GENERIC)
    {
        return -static_cast<int32_t>(error_type::OS_EIO);
    }
    return 0;
}

void pico_lcd_io::us_sleep(uint64_t us) const OS_NOEXCEPT
{
    os::us_sleep(us);
}



}
}

