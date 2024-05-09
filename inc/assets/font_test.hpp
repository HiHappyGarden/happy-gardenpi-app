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
namespace hhg {
    inline namespace v1 {

        namespace {
            uint8_t font_test[]
                    {
                            0x0C, 0x10, // font width, height
                            0xFF, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00,         //row0 = 12 pix x 8 pix
                            0xFF, 0x87, 0x85, 0x87, 0x80, 0x80, 0x80, 0x80, 0x41, 0x22, 0x14, 0x08   //row1 = 12 pix x 8 pix
                    };

        }
    }
}