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
using namespace os;

//#include "stm32g4xx_hal.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

namespace hhg::utils
{
inline namespace v1
{

int32_t random_number(int32_t min, int32_t max) OS_NOEXCEPT
{
//    srand(time(NULL));
//    return (rand() % (max - min + 1)) + min;
	return 0;
}

int32_t crc32(uint8_t buffer[], uint32_t buffer_len) OS_NOEXCEPT
{
    int i, j;
    unsigned int byte, crc, mask;

    i = 0;
    crc = 0xFFFFFFFF;
    while (i < buffer_len)
    {
        byte = buffer[i];            // Get next byte.
        crc = crc ^ byte;
        for (j = 7; j >= 0; j--)
        {    // Do eight times.
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (0xEDB88320 & mask);
        }
        i = i + 1;
    }
    return ~crc;
}

os::exit to_hex(char* dest, size_t dest_len, const uint8_t* values, size_t val_len) OS_NOEXCEPT
{
	if(dest == nullptr || values == nullptr)
	{
		return exit::KO;
	}
    if(dest_len < (val_len*2+1))
    {
    	return exit::KO;
    }

    memset(dest, '\0', dest_len);
    while(val_len--)
    {
        sprintf(dest, "%02X", *values);
        dest += 2;
        ++values;
    }
    return exit::OK;
}

os::exit from_hex(uint8_t* dest, size_t dest_len, const char* values, size_t val_len) OS_NOEXCEPT
{
	if(dest == nullptr || values == nullptr)
	{
		return exit::KO;
	}

	memset(dest, '\0', dest_len);
    char buf[3];
    size_t i;
    int value;
    for (i = 0; i < val_len && *values; i++)
    {
        buf[0] = *values++;
        buf[1] = '\0';
        if (*values)
        {
            buf[1] = *values++;
            buf[2] = '\0';
        }
        if (sscanf(buf, "%x", &value) != 1)
            break;
        dest[i] = value;
    }
    return exit::OK;
}

}
}
