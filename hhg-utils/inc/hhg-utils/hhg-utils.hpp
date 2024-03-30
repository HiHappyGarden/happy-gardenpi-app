/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

namespace hhg::utils
{
inline namespace v1
{

int32_t random_number(int32_t min, int32_t max) __attribute__ ((weak)) OS_NOEXCEPT;

int32_t crc32(const uint8_t buffer[], uint32_t buffer_len) OS_NOEXCEPT;

os::exit to_hex(char* dest, size_t dest_len, const uint8_t* src, size_t src_len) OS_NOEXCEPT;

os::exit from_hex(uint8_t* dest, size_t dest_len, const char* src, size_t src_len) OS_NOEXCEPT;

void print_hex(const uint8_t* buf, size_t len) OS_NOEXCEPT;

}
}
