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

#ifndef HHGARDEN_CONFIG_H
#define HHGARDEN_CONFIG_H

#define HHGARDEN_NAME "hi-happy-garden"
#define HHGARDEN_VER "0.52.0"
#define HHGARDEN_VER_MAJOR (0)
#define HHGARDEN_VER_MINOR (52)
#define HHGARDEN_VER_PATCH (0)
#define HHGARDEN_LCD_MSG_SIZE (34)
#define HHGARDEN_SCHEDULES_SIZE (8)
#define HHGARDEN_ZONES_SIZE (4)
#define HHGARDEN_HEADER (0xC1)
#define HHGARDEN_FILE_PATH_DATA "data.json"
#define HHGARDEN_FILE_PATH_CONF "conf.json"
#define HHGARDEN_FSM_MAIN_SLEEP (100)
#define HHGARDEN_FSM_ERROR_SLEEP (1000)
#define HHGARDEN_FSM_ERROR_MAX (5)
#define HHGARDEN_FSM_THREAD_PRIORITY (4)
#define HHGARDEN_FSM_THREAD_HEAP (8*1024)


#endif // HHGARDEN_CONFIG_H
