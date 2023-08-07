/***************************************************************************
 *
 * Hi Happy Garden Interfaces
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

enum class hhgd_type : int
{
    HHGD_LED_GREEN,
    HHGD_LED_RED,
    HHGD_RELAY_IN1,
    HHGD_RELAY_IN2,
    HHGD_RELAY_IN3,
    HHGD_RELAY_IN4,
    HHGD_BUTTON_NEXT,
    HHGD_BUTTON_BEFORE,
    HHGD_LCD,
    HHGD_NONE,
};

enum class error_code : uint16_t
{
    HHGD_INIT,
    HHGD_NO_HEAP,
    HHGD_HARDWARE_NO_DRIVER = 100,
    HHGD_HARDWARE_REGISTRATION,

};


constexpr const uint8_t SIGETX = 10;
constexpr const char HHGD_PATH[] = "/dev/hhgd";

}
}
