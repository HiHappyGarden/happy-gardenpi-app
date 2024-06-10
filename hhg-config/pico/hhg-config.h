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
#define HHG_VER "0.60.0"
#define HHG_VER_MAJOR (0)
#define HHG_VER_MINOR (60)
#define HHG_VER_PATCH (0)
#define HHG_DIVISOR "|"

#define HHG_SCHEDULES_SIZE (2)
#define HHG_ZONES_SIZE (4)

#define HHG_ADMIN_USER "admin"
#define HHG_ADMIN_PASSWD "admin"
#define HHG_USER "user"
#define HHG_PASSWD "user"
#if !defined(HHG_ADMIN_USER) || !defined(HHG_ADMIN_PASSWD)
#error HHG_ADMIN_USER or HHG_ADMIN_PASSWD are mandatory
#endif

#define HHG_WIFI_DISABLE (0)
#define HHG_WIFI_CONNECTION_TIMEOUT (10000)
#if HHG_WIFI_CONNECTION_TIMEOUT > 30000
#error HHG_WIFI_CONNECTION_TIMEOUT max value permitted 30000
#endif

#define HHG_NTP_MSG_LEN (48)
#define HHG_NTP_PORT (123)
#define HHG_NTP_SERVER "0.it.pool.ntp.org"

#endif // HHG_CONFIG_H
