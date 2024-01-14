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
, app_parser(hardware.get_io())
{

}

app_main::~app_main() OS_NOEXCEPT = default;

os::exit app_main::init(class os::error** error) OS_NOEXCEPT
{
	hardware.get_io()->set_on_receive(&app_parser, &hhg::iface::io_on_receive::on_receive);


	return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error** error) OS_NOEXCEPT
{
	return os::exit::OK;
}


}
}

