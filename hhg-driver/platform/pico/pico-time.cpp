/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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


#include "pico/pico-time.hpp"
using namespace os;

#include <pico/stdlib.h>
#include <pico/util/datetime.h>
#include <hardware/rtc.h>

namespace hhg::driver
{
inline namespace v1
{


pico_time::pico_time() = default;
pico_time::~pico_time() = default;

os::exit pico_time::init(error** error) OSAL_NOEXCEPT
{
    rtc_init();

    return exit::OK;
}

os::exit pico_time::set_timestamp(time_t timestamp, os::error **error) OSAL_NOEXCEPT
{
	const auto ts = gmtime(&timestamp);

    datetime_t t = {
            .year  = static_cast<int16_t>(ts->tm_year),
            .month = static_cast<int8_t>(ts->tm_mon),
            .day   = static_cast<int8_t>(ts->tm_mday),
            .dotw  = static_cast<int8_t>(ts->tm_wday), // 0 is Sunday, so 5 is Friday
            .hour  = static_cast<int8_t>(ts->tm_hour),
            .min   = static_cast<int8_t>(ts->tm_min),
            .sec   = static_cast<int8_t>(ts->tm_sec)
    };

    rtc_set_datetime(&t);

	return exit::OK;
}

::tm pico_time::get_date_time(int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT
{
    datetime_t t{0, 0, 0, 0, 0, 0, 0};
    rtc_get_datetime(&t);

	tm tm = {
		.tm_sec = t.sec,
		.tm_min = t.min,
		.tm_hour = t.hour,
		.tm_mday = t.day,
		.tm_mon = t.month,
		.tm_year = t.year,
		.tm_wday = t.dotw
	};

    if(is_dst(&tm))
    {
        tm.tm_hour++;
    }

    if(timezone)
    {
        tm.tm_min += timezone;
        if(tm.tm_min % 60)
        {
            tm.tm_min = tm.tm_min % 60;
            tm.tm_hour +=  tm.tm_min / 60;
        }
        if(tm.tm_hour % 24)
        {
            tm.tm_hour = tm.tm_hour % 24;
            tm.tm_hour +=  tm.tm_min / 60;
        }
        //TODO: da finire
    }

    return tm;
}

string<32> pico_time::get_date_time(const char format[], int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT
{
	string<32> ret;

	auto&& now = get_date_time(timezone, daylight_saving_time, error);

    if(is_dst(&now))
    {
        now.tm_hour++;
    }

	strftime(ret.c_str(), ret.size(), format, &now);

	ret.length();

	return ret;
}

bool pico_time::wait_for_synchro(uint64_t timeout) const OSAL_NOEXCEPT
{
	return true;
}

bool pico_time::is_dst(tm *timeinfo)
{
    tm start{}, end{};

    // Ultima domenica di marzo
    start = *timeinfo;
    start.tm_mon = 2; // Marzo
    start.tm_mday = 31;
    start.tm_hour = 2; // L'ora legale inizia alle 2:00 AM
    mktime(&start);
    while (start.tm_wday != 0)
    { // Finché non è domenica
        start.tm_mday--;
        mktime(&start);
    }

    // Ultima domenica di ottobre
    end = *timeinfo;
    end.tm_mon = 9; // Ottobre
    end.tm_mday = 31;
    end.tm_hour = 3; // L'ora legale termina alle 3:00 AM
    mktime(&end);
    while (end.tm_wday != 0)
    { // Finché non è domenica
        end.tm_mday--;
        mktime(&end);
    }

    if (difftime(mktime(timeinfo), mktime(&start)) >= 0 &&
        difftime(mktime(&end), mktime(timeinfo)) > 0) {
        return true; // È ora legale
    } else {
        return false; // Non è ora legale
    }
}

    inline constexpr int32_t pico_time::get_timezone_in_sec(int16_t imezone_in_minute)
    {
        return imezone_in_minute * 60;
    }


} /* namespace driver */
} /* namespace hhg */


