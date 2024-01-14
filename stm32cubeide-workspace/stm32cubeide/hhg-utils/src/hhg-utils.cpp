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

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

namespace hhg::utils
{
inline namespace v1
{


int32_t random_number(int32_t min, int32_t max) OS_NOEXCEPT
{
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}

}
}
