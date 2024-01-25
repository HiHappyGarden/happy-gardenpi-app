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

#include "hhg-app/app-data.hpp"
using namespace osal;
using hhg::iface::fs_io;
using hhg::iface::data_type;

#include "hhg-utils/hhg-utils.hpp"
using hhg::utils::crc32;

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_data::app_data(const fs_io::ptr& fsio) OS_NOEXCEPT
: fsio(fsio)
{

}

app_data::~app_data() = default;

inline os::exit app_data::init(os::error** error) OS_NOEXCEPT
{
	return load(nullptr, error);
}

void app_data::reset() OS_NOEXCEPT
{
	data = {};
}

os::exit app_data::store(os::error** error) const OS_NOEXCEPT
{
	data.crc = crc32(reinterpret_cast<uint8_t *>(&data), sizeof(data));
	return fsio->write(data_type::DATA, reinterpret_cast<const uint8_t *>(&data), sizeof(data), error);
}

os::exit app_data::load(app_data::on_vesrion_change on_vesrion_change, os::error** error) OS_NOEXCEPT
{
	class data local_data;

	if(fsio->read(data_type::DATA, reinterpret_cast<uint8_t *>(&local_data), sizeof(local_data), error) == exit::KO)
	{
		return exit::KO;
	}

	uint32_t original_crc = local_data.crc;
	local_data.crc = MAIGC;
	uint32_t crc = crc32(reinterpret_cast<uint8_t *>(&local_data), sizeof(local_data));
	local_data.crc = original_crc;

	if(crc != original_crc)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() crc error.", error_type::OS_ERCRC);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(local_data.magic != MAIGC)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() magic number error.", error_type::OS_EBADF);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(on_vesrion_change)
	{
		on_vesrion_change(local_data.version);
	}

	data = local_data;

	return exit::OK;
}

os::exit app_data::load_defaut(os::error** error) OS_NOEXCEPT
{
	reset();
	return store(error);
}

} /* namespace driver */
} /* namespace hhg */
