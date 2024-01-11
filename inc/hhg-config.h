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

#ifndef HHG_CONFIG_H
#define HHG_CONFIG_H

#define HHG_NAME "hi-happy-garden"
#define HHG_VER "0.52.0"
#define HHG_VER_MAJOR (0)
#define HHG_VER_MINOR (52)
#define HHG_VER_PATCH (0)
#define HHG_LCD_MSG_SIZE (34)
#define HHG_SCHEDULES_SIZE (8)
#define HHG_ZONES_SIZE (4)
#define HHG_HEADER (0xC1)
#define HHG_FILE_PATH_DATA "data.json"
#define HHG_FILE_PATH_CONF "conf.json"
#define HHG_FSM_MAIN_SLEEP (100)
#define HHG_FSM_ERROR_SLEEP (1000)
#define HHG_FSM_ERROR_MAX (5)
#define HHG_FSM_THREAD_PRIORITY (4)
#define HHG_FSM_THREAD_HEAP (8*1024)


#endif // HHG_CONFIG_H
