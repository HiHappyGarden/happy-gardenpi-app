/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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
#include "hhg-iface/initializable.hpp"

namespace hhg::driver
{
inline namespace v1
{



class pico_time final : public hhg::iface::time_init
{
public:
	pico_time();
	~pico_time() override;
	OSAL_NO_COPY_NO_MOVE(pico_time)

    os::exit init(os::error** error) OSAL_NOEXCEPT override;

	os::exit set_timestamp(time_t timestamp, os::error **error) OSAL_NOEXCEPT override;

	::tm get_date_time(int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT override;

	os::string<32> get_date_time(const char[], int16_t timezone, bool daylight_saving_time, os::error **error) const OSAL_NOEXCEPT override;

    os::string<32> to_string(time_t timestamp, const char format[], int16_t timezone, bool daylight_saving_time) const OSAL_NOEXCEPT override;

private:
    static bool is_dst(tm *time) OSAL_NOEXCEPT;
    static void add_minute(tm* tm, time_t minute) OSAL_NOEXCEPT;
    static void add_hour(tm* tm, int8_t hours) OSAL_NOEXCEPT;
};

} /* namespace driver */
} /* namespace hhg */

