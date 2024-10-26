/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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


namespace hhg::utils
{
inline namespace v1
{

int32_t crc32(const uint8_t buffer[], uint32_t buffer_len) OSAL_NOEXCEPT
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

os::exit to_hex(char* dest, size_t dest_len, const uint8_t* src, size_t src_len) OSAL_NOEXCEPT
{
	if(dest == nullptr || src == nullptr)
	{
		return exit::KO;
	}
    if(dest_len < (src_len * 2 + 1))
    {
    	return exit::KO;
    }

    memset(dest, '\0', dest_len);
    while(src_len--)
    {
        sprintf(dest, "%02X", *src);
        dest += 2;
        ++src;
    }
    return exit::OK;
}

os::exit from_hex(uint8_t* dest, size_t dest_len, const char* src, size_t src_len) OSAL_NOEXCEPT
{
	if(dest == nullptr || src == nullptr)
	{
		return exit::KO;
	}

	memset(dest, '\0', dest_len);
    char buf[3];
    size_t i;
    int value;
    for (i = 0; i < src_len && *src; i++)
    {
        buf[0] = *src++;
        buf[1] = '\0';
        if (*src)
        {
            buf[1] = *src++;
            buf[2] = '\0';
        }
        if (sscanf(buf, "%x", &value) != 1)
            break;
        dest[i] = value;
    }
    return exit::OK;
}

void print_hex(const uint8_t* buf, size_t len) OSAL_NOEXCEPT
{
    for (size_t i = 0; i < len; ++i)
    {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}


}
}
