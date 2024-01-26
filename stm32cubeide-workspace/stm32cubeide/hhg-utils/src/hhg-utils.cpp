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

#include "hhg-utils/hhg-utils.hpp"
#include "stm32g4xx_hal.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

namespace hhg::utils
{
inline namespace v1
{

extern "C" CRC_HandleTypeDef hcrc;
extern "C" uint32_t HAL_CRC_Calculate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);


int32_t random_number(int32_t min, int32_t max) OS_NOEXCEPT
{
//    srand(time(NULL));
//    return (rand() % (max - min + 1)) + min;
	return 0;
}

int32_t crc32(uint8_t buffer[], uint32_t buffer_len) OS_NOEXCEPT
{
	return HAL_CRC_Calculate(&hcrc, reinterpret_cast<uint32_t*>(buffer), buffer_len);
}

}
}
