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


#include "stm32g4xx/stm32-time.hpp"
using namespace os;

#include "stm32g4xx.h"
#include "stm32g4xx_ll_rtc.h"

extern "C" RTC_HandleTypeDef hrtc;

namespace hhg::driver
{
inline namespace v1
{


stm32_time::stm32_time() = default;
stm32_time::~stm32_time() = default;


os::exit stm32_time::set_timestamp(time_t timestamp, os::error **error) OS_NOEXCEPT
{
	RTC_TimeTypeDef local_time = {0};
	RTC_DateTypeDef local_date = {0};

	tm ts = *gmtime(&timestamp);

	local_time.Hours = ts.tm_hour + 1;
	local_time.Minutes = ts.tm_min;
	local_time.Seconds = ts.tm_sec;
	local_time.SubSeconds = 0x0;
	local_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	local_time.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &local_time, RTC_FORMAT_BCD) != HAL_OK)
	{
        if(error)
        {
            *error = OS_ERROR_BUILD("Invalid HAL_RTC_SetTime()", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
		return exit::KO;
	}
	local_date.Year = ts.tm_year - 100; //start 1900
	local_date.Month = ts.tm_mon + 1;
	local_date.WeekDay = ts.tm_wday + 1;
	local_date.Date = ts.tm_mday;

	if (HAL_RTC_SetDate(&hrtc, &local_date, RTC_FORMAT_BCD) != HAL_OK)
	{
        if(error)
        {
            *error = OS_ERROR_BUILD("Invalid HAL_RTC_SetDate()", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
		return exit::KO;
	}
	return exit::OK;
}

::tm stm32_time::get_date_time(os::error **error) const OS_NOEXCEPT
{
	RTC_TimeTypeDef local_time = {0};
	RTC_DateTypeDef local_date = {0};

	if(HAL_RTC_GetTime(&hrtc, &local_time, RTC_FORMAT_BCD) != HAL_OK)
	{
        if(error)
        {
            *error = OS_ERROR_BUILD("Invalid HAL_RTC_GetTime()", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
		return {0};
	}

	if(HAL_RTC_GetDate(&hrtc, &local_date, RTC_FORMAT_BCD) != HAL_OK)
	{
        if(error)
        {
            *error = OS_ERROR_BUILD("Invalid HAL_RTC_GetTime()", error_type::OS_EINVAL);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return {0};
	}

	return {
		.tm_sec = local_time.Seconds,
		.tm_min = local_time.Minutes,
		.tm_hour = local_time.Hours - 1,
		.tm_mday = local_date.Date,
		.tm_mon = local_date.Month - 1,
		.tm_year = local_date.Year + 100,
		.tm_wday = local_date.WeekDay - 1
	};
}

string<32> stm32_time::get_date_time(const char format[], os::error **error) const OS_NOEXCEPT
{
	string<32> ret;

	auto&& now = get_date_time(error);

	strftime(ret.c_str(), ret.size(), format, &now);

	ret.length();

	return ret;
}

bool stm32_time::wait_for_synchro(uint64_t timeout) const OS_NOEXCEPT
{
	/* Clear RSF flag */
	LL_RTC_ClearFlag_RS(RTC);

	/* Wait the registers to be synchronised */
	while (LL_RTC_IsActiveFlag_RS(RTC) != 1)
	{
#if (USE_TIMEOUT == 1)
		if (LL_SYSTICK_IsActiveCounterFlag())
		{
		  timeout --;
		}
		if (timeout == 0)
		{
			return false;
		}
#endif /* USE_TIMEOUT */
	}
	return true;
}

} /* namespace driver */
} /* namespace hhg */


