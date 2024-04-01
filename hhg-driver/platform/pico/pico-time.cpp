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

os::exit pico_time::init(error** error) OS_NOEXCEPT
{
    rtc_init();

    return exit::OK;
}

os::exit pico_time::set_timestamp(time_t timestamp, os::error **error) OS_NOEXCEPT
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

::tm pico_time::get_date_time(os::error **error) const OS_NOEXCEPT
{
    datetime_t t{0, 0, 0, 0, 0, 0, 0};
    rtc_get_datetime(&t);

	return {
		.tm_sec = t.sec,
		.tm_min = t.min,
		.tm_hour = t.hour,
		.tm_mday = t.day,
		.tm_mon = t.month,
		.tm_year = t.year,
		.tm_wday = t.dotw
	};
}

string<32> pico_time::get_date_time(const char format[], os::error **error) const OS_NOEXCEPT
{
	string<32> ret;

	auto&& now = get_date_time(error);

	strftime(ret.c_str(), ret.size(), format, &now);

	ret.length();

	return ret;
}

bool pico_time::wait_for_synchro(uint64_t timeout) const OS_NOEXCEPT
{
	return true;
}


} /* namespace driver */
} /* namespace hhg */


