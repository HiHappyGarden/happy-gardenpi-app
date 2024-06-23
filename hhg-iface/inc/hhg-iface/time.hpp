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


#pragma once

#include "osal/osal.hpp"
#include "hhg-iface/initializable.hpp"


#include <time.h>

namespace hhg::iface
{
inline namespace v1
{



struct time
{
	static constexpr char FORMAT[] = "%Y/%m/%d  %H:%M";

	using ptr = os::unique_ptr<hhg::iface::time>;

	virtual ~time() = default;

	virtual os::exit set_timestamp(time_t timestamp, os::error **error) OSAL_NOEXCEPT = 0;

	virtual ::tm get_date_time(int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT = 0;

	virtual inline time_t get_timestamp(int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT
	{
		tm&& ret = get_date_time(timezone, daylight_saving_time, error);
		return mktime(&ret);
	}


	virtual os::string<32> get_date_time(const char format[], int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT = 0;

	virtual bool wait_for_synchro(uint64_t timeout) const OSAL_NOEXCEPT { return true; };

    virtual  os::string<32> to_string(time_t timestamp, const char format[], int16_t timezone, bool daylight_saving_time) const OSAL_NOEXCEPT = 0;
};

struct time_init : public time, public initializable
{
    ~time_init() override = default;

};

} /* namespace driver */
} /* namespace hhg */
