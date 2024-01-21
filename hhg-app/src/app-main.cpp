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

#include <hhg-app/app-main.hpp>
using namespace os;

#include "hhg-app/app-parser-commands.hpp"

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "APP MAIN";

}

app_main::app_main(driver::hardware& hardware) OS_NOEXCEPT
: hardware(hardware)
, app_config(hardware.get_fsio())
, app_parser(hardware.get_io())
{

}

app_main::~app_main() OS_NOEXCEPT = default;

os::exit app_main::init(class os::error** error) OS_NOEXCEPT
{
	OS_LOG_INFO(APP_TAG, "Init APP PARSER");
	hardware.get_io()->set_on_receive(&app_parser, &hhg::iface::io_on_receive::on_receive);
	set_app_parser(app_parser);
	OS_LOG_INFO(APP_TAG, "Init APP PARSER - OK");


	OS_LOG_INFO(APP_TAG, "Init APP CONFIG");
	if(app_config.init(error) == exit::KO)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::init() fail.", error_type::OS_EFAULT);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	set_app_config(app_config);
	OS_LOG_INFO(APP_TAG, "Init APP CONFIG - OK");




	return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error** error) OS_NOEXCEPT
{
	return os::exit::OK;
}


}
}

