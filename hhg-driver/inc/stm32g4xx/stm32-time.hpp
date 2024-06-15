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

#include "hhg-iface/time.hpp"

namespace hhg::driver
{
inline namespace v1
{



class stm32_time final : public hhg::iface::time
{
public:
	static constexpr time_t TIMESTAMP_2000 = 946688400;

	stm32_time();
	~stm32_time();
	OS_NO_COPY_NO_MOVE(stm32_time)

	os::exit set_timestamp(time_t timestamp, os::error **error = nullptr) OSAL_NOEXCEPT override;

	::tm get_date_time(os::error **error = nullptr) const OSAL_NOEXCEPT override;

	os::string<32> get_date_time(const char[] = FORMAT, os::error **error = nullptr) const OSAL_NOEXCEPT override;

	bool wait_for_synchro(uint64_t timeout = os::ms_to_us(1'000)) const OSAL_NOEXCEPT override;

};

} /* namespace driver */
} /* namespace hhg */

