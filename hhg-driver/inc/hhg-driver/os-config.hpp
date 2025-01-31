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

#pragma once
#include "osal/osal.hpp"

#include <stdint.h>

namespace hhg::driver
{
inline namespace v1
{

extern uint32_t const LOWEST;
extern uint32_t const LOW;
extern uint32_t const NORMAL;
extern uint32_t const HIGH;
extern uint32_t const HIGHEST;
extern uint32_t const REALTIME;

extern uint32_t const MINIMAL_STACK_SIZE;

extern char const TIME_T_STR[];

os::exit os_config_init() OSAL_NOEXCEPT;

void print_memory_status(char task_table[], size_t task_table_size, char current_task_name[], uint16_t current_task_name_size, size_t& heap_free, size_t& stack_free);

void reboot() OSAL_NOEXCEPT;

}
}



