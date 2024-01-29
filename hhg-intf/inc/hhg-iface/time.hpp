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


#include <time.h>

namespace hhg::iface
{
inline namespace v1
{



class time
{

public:
	using ptr = os::unique_ptr<hhg::iface::time>;

	virtual ~time() = default;

	virtual os::exit set_timestamp(time_t timestamp, os::error **error = nullptr) OS_NOEXCEPT = 0;

	virtual ::tm get_time(os::error **error = nullptr) const OS_NOEXCEPT = 0;

	virtual inline time_t get_timestamp(os::error **error = nullptr) const OS_NOEXCEPT
	{
		tm&& ret = get_time(error);
		return mktime(&ret);
	}

	virtual bool wait_for_synchro(uint64_t timeout = os::ms_to_us(1'000)) const OS_NOEXCEPT { return true; };

};

} /* namespace driver */
} /* namespace hhg */
