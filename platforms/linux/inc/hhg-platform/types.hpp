/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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
#include <stdint.h>

namespace hhg::platform
{
inline namespace v1
{

enum class type : uint32_t
{
    LED_GREEN,
    LED_RED,
    RELAY_IN1,
    RELAY_IN2,
    RELAY_IN3,
    RELAY_IN4,
    BUTTON_NEXT,
    BUTTON_BEFORE,
    LCD,
    NONE,
    MASK = 0x0F,
};

enum class action : uint8_t
{
    READ = 0x40,
    WRITE = 0x80,
};


constexpr const uint8_t SIGETX = 10;
constexpr const char HHGD_PATH[] = "/dev/hhgd";

}
}
