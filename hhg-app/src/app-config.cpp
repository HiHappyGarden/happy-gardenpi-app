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

#include "hhg-app/app-config.hpp"
using namespace osal;
using hhg::iface::fsio;
using hhg::iface::data_type;


namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_config::app_config(const fsio::ptr& fsio) OS_NOEXCEPT
: fsio(fsio)
{

}

app_config::~app_config() = default;

os::exit app_config::init(os::error** error) OS_NOEXCEPT
{

	return os::exit::OK;
}

os::exit app_config::set_serial(const char serial[]) OS_NOEXCEPT
{
	if(serial)
		config.serial += serial;
	else
		config.serial = "";

	error* error = nullptr;
	if(fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

os::exit app_config::set_descr(const char descr[]) OS_NOEXCEPT
{
	if(descr)
		config.descr += descr;
	else
		config.descr = "";

	error* error = nullptr;
	if(fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t*>(&config), sizeof(config), &error) == exit::KO)
	{
		printf_stack_error(APP_TAG, error);
		delete error;
		return exit::KO;
	}

	return exit::OK;
}

const char* app_config::get_version() const OS_NOEXCEPT
{
	static string<16> ret;
	if(ret.length() == 0)
	{
		snprintf(ret.c_str(), ret.size(), "%u.%u.%u", config.version_major, config.version_minor, config.version_patch);
	}
	return ret.c_str();
}

os::exit app_config::store(os::error** error) const OS_NOEXCEPT
{
	return fsio->write(data_type::CONFIG, reinterpret_cast<const uint8_t *>(&config), sizeof(config), error);
}

} /* namespace driver */
} /* namespace hhg */
