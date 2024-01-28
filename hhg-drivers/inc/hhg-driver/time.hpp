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

namespace hhg::driver
{
inline namespace v1
{



class time final
{

	os::string<16> format;
public:
	static constexpr const char DEFAULT_FORMAT[] = "%u/%u/%u %u:%u";

	explicit time(const char format[] = DEFAULT_FORMAT);
	~time();
	OS_NO_COPY_NO_MOVE(time)

	os::exit set_rtc(time_t timestamp, os::error **error = nullptr) OS_NOEXCEPT;

	::tm get_rtc(os::error **error = nullptr) const OS_NOEXCEPT;

	bool wait_for_synchro(uint64_t timeout = os::ms_to_us(1'000)) const OS_NOEXCEPT;

};

} /* namespace driver */
} /* namespace hhg */

