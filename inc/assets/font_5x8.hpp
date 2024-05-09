/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023-2024  Antonio Salsi <passy.linux@zresa.it>
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
namespace hhg
{
inline namespace v1
{

namespace
{

    constexpr uint8_t  font_5x8[] =
    {
    0x5, 0x8, // font width, height

    0x0,
    0x0,
    0x0,
    0x0,
    0x0,

    0x0,
    0x0,
    0x5c,
    0x0,
    0x0,

    0x0,
    0xc,
    0x0,
    0xc,
    0x0,

    0x28,
    0x7c,
    0x28,
    0x7c,
    0x28,

    0x0,
    0x50,
    0xec,
    0x28,
    0x0,

    0x44,
    0x2a,
    0x34,
    0x58,
    0x24,

    0x20,
    0x58,
    0x54,
    0x24,
    0x50,

    0x0,
    0x0,
    0x6,
    0x0,
    0x0,

    0x0,
    0x38,
    0x44,
    0x0,
    0x0,

    0x0,
    0x44,
    0x38,
    0x0,
    0x0,

    0x0,
    0x54,
    0x38,
    0x54,
    0x0,

    0x0,
    0x10,
    0x38,
    0x10,
    0x0,

    0x0,
    0x80,
    0x40,
    0x0,
    0x0,

    0x8,
    0x8,
    0x8,
    0x8,
    0x0,

    0x0,
    0x0,
    0x40,
    0x0,
    0x0,

    0x0,
    0x60,
    0x18,
    0x4,
    0x0,

    0x38,
    0x44,
    0x44,
    0x38,
    0x0,

    0x0,
    0x8,
    0x7c,
    0x0,
    0x0,

    0x48,
    0x64,
    0x54,
    0x48,
    0x0,

    0x44,
    0x54,
    0x54,
    0x28,
    0x0,

    0x20,
    0x30,
    0x28,
    0x7c,
    0x0,

    0x5c,
    0x54,
    0x54,
    0x24,
    0x0,

    0x38,
    0x54,
    0x54,
    0x20,
    0x0,

    0x4,
    0x64,
    0x14,
    0xc,
    0x0,

    0x28,
    0x54,
    0x54,
    0x28,
    0x0,

    0x8,
    0x54,
    0x54,
    0x38,
    0x0,

    0x0,
    0x0,
    0x50,
    0x0,
    0x0,

    0x0,
    0x80,
    0x50,
    0x0,
    0x0,

    0x0,
    0x10,
    0x28,
    0x44,
    0x0,

    0x0,
    0x28,
    0x28,
    0x28,
    0x0,

    0x0,
    0x44,
    0x28,
    0x10,
    0x0,

    0x0,
    0x54,
    0x14,
    0x8,
    0x0,

    0x38,
    0x44,
    0x54,
    0x54,
    0x8,

    0x78,
    0x14,
    0x14,
    0x78,
    0x0,

    0x7c,
    0x54,
    0x54,
    0x28,
    0x0,

    0x38,
    0x44,
    0x44,
    0x44,
    0x0,

    0x7c,
    0x44,
    0x44,
    0x38,
    0x0,

    0x7c,
    0x54,
    0x54,
    0x44,
    0x0,

    0x7c,
    0x14,
    0x14,
    0x4,
    0x0,

    0x38,
    0x44,
    0x44,
    0x68,
    0x0,

    0x7c,
    0x10,
    0x10,
    0x7c,
    0x0,

    0x0,
    0x44,
    0x7c,
    0x44,
    0x0,

    0x30,
    0x40,
    0x40,
    0x3c,
    0x0,

    0x7c,
    0x10,
    0x28,
    0x44,
    0x0,

    0x7c,
    0x40,
    0x40,
    0x40,
    0x0,

    0x7c,
    0x10,
    0x10,
    0x7c,
    0x0,

    0x7c,
    0x8,
    0x10,
    0x7c,
    0x0,

    0x38,
    0x44,
    0x44,
    0x38,
    0x0,

    0x7c,
    0x14,
    0x14,
    0x8,
    0x0,

    0x38,
    0x44,
    0x44,
    0xb8,
    0x0,

    0x7c,
    0x14,
    0x14,
    0x68,
    0x0,

    0x48,
    0x54,
    0x54,
    0x24,
    0x0,

    0x4,
    0x4,
    0x7c,
    0x4,
    0x4,

    0x3c,
    0x40,
    0x40,
    0x3c,
    0x0,

    0x1c,
    0x60,
    0x60,
    0x1c,
    0x0,

    0x1c,
    0x60,
    0x18,
    0x60,
    0x1c,

    0x4c,
    0x30,
    0x10,
    0x6c,
    0x0,

    0x0,
    0x1c,
    0x60,
    0x1c,
    0x0,

    0x64,
    0x54,
    0x4c,
    0x44,
    0x0,

    0x0,
    0x7c,
    0x44,
    0x0,
    0x0,

    0x0,
    0xc,
    0x30,
    0x40,
    0x0,

    0x0,
    0x44,
    0x7c,
    0x0,
    0x0,

    0x0,
    0x8,
    0x4,
    0x8,
    0x0,

    0x80,
    0x80,
    0x80,
    0x80,
    0x80,

    0x0,
    0x4,
    0x8,
    0x0,
    0x0,

    0x0,
    0x68,
    0x28,
    0x70,
    0x0,

    0x7e,
    0x48,
    0x48,
    0x30,
    0x0,

    0x0,
    0x30,
    0x48,
    0x48,
    0x0,

    0x30,
    0x48,
    0x48,
    0x7c,
    0x0,

    0x30,
    0x58,
    0x58,
    0x50,
    0x0,

    0x10,
    0x78,
    0x14,
    0x4,
    0x0,

    0x10,
    0xa8,
    0xa8,
    0x78,
    0x0,

    0x7c,
    0x8,
    0x8,
    0x70,
    0x0,

    0x0,
    0x48,
    0x7a,
    0x40,
    0x0,

    0x0,
    0x80,
    0x80,
    0x7a,
    0x0,

    0x7c,
    0x10,
    0x28,
    0x40,
    0x0,

    0x0,
    0x42,
    0x7e,
    0x40,
    0x0,

    0x78,
    0x10,
    0x10,
    0x78,
    0x0,

    0x78,
    0x8,
    0x8,
    0x70,
    0x0,

    0x30,
    0x48,
    0x48,
    0x30,
    0x0,

    0xf8,
    0x48,
    0x48,
    0x30,
    0x0,

    0x30,
    0x48,
    0x48,
    0xf8,
    0x0,

    0x0,
    0x78,
    0x10,
    0x8,
    0x0,

    0x50,
    0x58,
    0x68,
    0x28,
    0x0,

    0x8,
    0x3c,
    0x48,
    0x48,
    0x0,

    0x38,
    0x40,
    0x40,
    0x78,
    0x0,

    0x18,
    0x60,
    0x60,
    0x18,
    0x0,

    0x78,
    0x20,
    0x20,
    0x78,
    0x0,

    0x48,
    0x30,
    0x30,
    0x48,
    0x0,

    0x18,
    0xa0,
    0xa0,
    0x78,
    0x0,

    0x48,
    0x68,
    0x58,
    0x48,
    0x0,

    0x0,
    0x18,
    0x24,
    0x42,
    0x0,

    0x0,
    0x0,
    0x7e,
    0x0,
    0x0,

    0x0,
    0x42,
    0x24,
    0x18,
    0x0,

    0x10,
    0x8,
    0x10,
    0x8,
    0x0,

    0x60,
    0x50,
    0x48,
    0x50,
    0x60,

    0x38,
    0x44,
    0xc4,
    0x44,
    0x0,

    0x38,
    0x42,
    0x40,
    0x7a,
    0x0,

    0x30,
    0x58,
    0x5a,
    0x51,
    0x0,

    0x28,
    0x4a,
    0x31,
    0x42,
    0x0,

    0x48,
    0x2a,
    0x70,
    0x42,
    0x0,

    0x48,
    0x29,
    0x72,
    0x40,
    0x0,

    0x48,
    0x28,
    0x72,
    0x40,
    0x0,

    0x0,
    0x30,
    0xc8,
    0x48,
    0x0,

    0x30,
    0x5a,
    0x59,
    0x52,
    0x0,

    0x30,
    0x5a,
    0x58,
    0x52,
    0x0,

    0x30,
    0x59,
    0x5a,
    0x50,
    0x0,

    0x0,
    0x4a,
    0x78,
    0x42,
    0x0,

    0x0,
    0x4a,
    0x79,
    0x42,
    0x0,

    0x0,
    0x49,
    0x7a,
    0x40,
    0x0,

    0x79,
    0x14,
    0x15,
    0x78,
    0x0,

    0x78,
    0x14,
    0x15,
    0x78,
    0x0,

    0x7c,
    0x54,
    0x56,
    0x45,
    0x0,

    0x68,
    0x38,
    0x70,
    0x58,
    0x58,

    0x78,
    0x14,
    0x7c,
    0x54,
    0x0,

    0x30,
    0x4a,
    0x49,
    0x32,
    0x0,

    0x30,
    0x4a,
    0x48,
    0x32,
    0x0,

    0x30,
    0x49,
    0x4a,
    0x30,
    0x0,

    0x38,
    0x42,
    0x41,
    0x7a,
    0x0,

    0x38,
    0x41,
    0x42,
    0x78,
    0x0,

    0x18,
    0xa2,
    0xa0,
    0x7a,
    0x0,

    0x30,
    0x4a,
    0x48,
    0x32,
    0x0,

    0x3c,
    0x41,
    0x40,
    0x3d,
    0x0,

    0x30,
    0x48,
    0xcc,
    0x48,
    0x0,

    0x50,
    0x7c,
    0x52,
    0x46,
    0x0,

    0x2,
    0x2e,
    0x70,
    0x2e,
    0x2,

    0x7e,
    0x12,
    0x1c,
    0x38,
    0x50,

    0x90,
    0x7c,
    0x12,
    0x12,
    0x0,

    0x48,
    0x2a,
    0x71,
    0x40,
    0x0,

    0x0,
    0x48,
    0x7a,
    0x41,
    0x0,

    0x30,
    0x48,
    0x4a,
    0x31,
    0x0,

    0x38,
    0x40,
    0x42,
    0x79,
    0x0,

    0x7a,
    0x9,
    0xa,
    0x71,
    0x0,

    0x7e,
    0x19,
    0x22,
    0x7d,
    0x0,

    0x0,
    0x24,
    0x2a,
    0x2c,
    0x0,

    0x0,
    0x24,
    0x2a,
    0x24,
    0x0,

    0x20,
    0x50,
    0x4a,
    0x20,
    0x0,

    0x60,
    0x20,
    0x20,
    0x20,
    0x20,

    0x20,
    0x20,
    0x20,
    0x20,
    0x60,

    0x2e,
    0x10,
    0x48,
    0x54,
    0x70,

    0x2e,
    0x10,
    0x48,
    0x64,
    0xf2,

    0x0,
    0x20,
    0x7a,
    0x20,
    0x0,

    0x20,
    0x50,
    0x20,
    0x50,
    0x0,

    0x50,
    0x20,
    0x50,
    0x20,
    0x0,

    0x55,
    0xaa,
    0x55,
    0xaa,
    0x55,

    0x55,
    0xbb,
    0x55,
    0xee,
    0x55,

    0x55,
    0xff,
    0xaa,
    0xff,
    0x55,

    0x0,
    0x0,
    0xff,
    0x0,
    0x0,

    0x8,
    0x8,
    0xff,
    0x0,
    0x0,

    0x14,
    0x14,
    0xff,
    0x0,
    0x0,

    0x8,
    0xff,
    0x0,
    0xff,
    0x0,

    0x8,
    0xf8,
    0x8,
    0xf8,
    0x0,

    0x14,
    0x14,
    0xfc,
    0x0,
    0x0,

    0x14,
    0xf7,
    0x0,
    0xff,
    0x0,

    0x0,
    0xff,
    0x0,
    0xff,
    0x0,

    0x14,
    0xf4,
    0x4,
    0xfc,
    0x0,

    0x14,
    0x17,
    0x10,
    0x1f,
    0x0,

    0x8,
    0xf,
    0x8,
    0xf,
    0x0,

    0x14,
    0x14,
    0x1f,
    0x0,
    0x0,

    0x8,
    0x8,
    0xf8,
    0x0,
    0x0,

    0x0,
    0x0,
    0xf,
    0x8,
    0x8,

    0x8,
    0x8,
    0xf,
    0x8,
    0x8,

    0x8,
    0x8,
    0xf8,
    0x8,
    0x8,

    0x0,
    0x0,
    0xff,
    0x8,
    0x8,

    0x8,
    0x8,
    0x8,
    0x8,
    0x8,

    0x8,
    0x8,
    0xff,
    0x8,
    0x8,

    0x0,
    0x0,
    0xff,
    0x14,
    0x14,

    0x0,
    0xff,
    0x0,
    0xff,
    0x8,

    0x0,
    0x1f,
    0x10,
    0x17,
    0x14,

    0x0,
    0xfc,
    0x4,
    0xf4,
    0x14,

    0x14,
    0x17,
    0x10,
    0x17,
    0x14,

    0x14,
    0xf4,
    0x4,
    0xf4,
    0x14,

    0x0,
    0xff,
    0x0,
    0xf7,
    0x14,

    0x14,
    0x14,
    0x14,
    0x14,
    0x14,

    0x14,
    0xf7,
    0x0,
    0xf7,
    0x14,

    0x14,
    0x14,
    0x17,
    0x14,
    0x14,

    0x8,
    0xf,
    0x8,
    0xf,
    0x8,

    0x14,
    0x14,
    0xf4,
    0x14,
    0x14,

    0x8,
    0xf8,
    0x8,
    0xf8,
    0x8,

    0x0,
    0xf,
    0x8,
    0xf,
    0x8,

    0x0,
    0x0,
    0x1f,
    0x14,
    0x14,

    0x0,
    0x0,
    0xfc,
    0x14,
    0x14,

    0x0,
    0xf8,
    0x8,
    0xf8,
    0x8,

    0x8,
    0xff,
    0x8,
    0xff,
    0x8,

    0x14,
    0x14,
    0xff,
    0x14,
    0x14,

    0x8,
    0x8,
    0xf,
    0x0,
    0x0,

    0x0,
    0x0,
    0xf8,
    0x8,
    0x8,

    0xff,
    0xff,
    0xff,
    0xff,
    0xff,

    0xf0,
    0xf0,
    0xf0,
    0xf0,
    0xf0,

    0xff,
    0xff,
    0xff,
    0x0,
    0x0,

    0x0,
    0x0,
    0x0,
    0xff,
    0xff,

    0xf,
    0xf,
    0xf,
    0xf,
    0xf,

    0x30,
    0x48,
    0x48,
    0x30,
    0x48,

    0xfc,
    0x4a,
    0x4a,
    0x3c,
    0x0,

    0x0,
    0x7e,
    0x2,
    0x2,
    0x0,

    0x0,
    0x7c,
    0x4,
    0x7c,
    0x0,

    0x62,
    0x56,
    0x4a,
    0x42,
    0x66,

    0x38,
    0x44,
    0x44,
    0x3c,
    0x4,

    0xf8,
    0x40,
    0x40,
    0x38,
    0x40,

    0x2,
    0x4,
    0x78,
    0x6,
    0x2,

    0x10,
    0x28,
    0xee,
    0x28,
    0x10,

    0x38,
    0x54,
    0x54,
    0x54,
    0x38,

    0x58,
    0x64,
    0x4,
    0x64,
    0x58,

    0x32,
    0x4d,
    0x49,
    0x30,
    0x0,

    0x30,
    0x48,
    0x78,
    0x48,
    0x30,

    0x50,
    0x28,
    0x58,
    0x48,
    0x34,

    0x0,
    0x3c,
    0x4a,
    0x4a,
    0x0,

    0x7c,
    0x2,
    0x2,
    0x7c,
    0x0,

    0x54,
    0x54,
    0x54,
    0x54,
    0x0,

    0x48,
    0x48,
    0x5c,
    0x48,
    0x48,

    0x40,
    0x62,
    0x54,
    0x48,
    0x0,

    0x0,
    0x48,
    0x54,
    0x62,
    0x0,

    0x0,
    0x0,
    0xf8,
    0x4,
    0xc,

    0x30,
    0x20,
    0x1f,
    0x0,
    0x0,

    0x10,
    0x54,
    0x54,
    0x10,
    0x0,

    0x48,
    0x24,
    0x48,
    0x24,
    0x0,

    0x0,
    0x8,
    0x14,
    0x8,
    0x0,

    0x0,
    0x18,
    0x18,
    0x0,
    0x0,

    0x0,
    0x0,
    0x8,
    0x0,
    0x0,

    0x20,
    0x40,
    0x30,
    0xc,
    0x4,

    0x0,
    0xe,
    0x2,
    0xc,
    0x0,

    0x0,
    0x12,
    0x1a,
    0x14,
    0x0,

    0x0,
    0x38,
    0x38,
    0x38,
    0x0,

    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    };
}

}
}
