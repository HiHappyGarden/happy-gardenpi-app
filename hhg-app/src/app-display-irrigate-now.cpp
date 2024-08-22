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

#include "hhg-app/app-display-irrigate-now.hpp"
#include "hhg-app/app-display-handler.hpp"
#include "hhg-app/app-parser.hpp"
using namespace os;
using hhg::iface::button;
using hhg::iface::event_exit;

namespace hhg::app
{
inline namespace v1
{


app_display_irrigate_now::app_display_irrigate_now(class app_display_handler& app_display_handler, const class app_parser& app_parser, int16_t& menu_idx, hhg::iface::event_exit* obj, hhg::iface::event_exit::on_exit_callback on_exit)
: app_display_handler(app_display_handler)
, app_parser(app_parser)
, menu_idx(menu_idx)
, obj(obj)
, on_exit_callback(on_exit)
{

}

void app_display_irrigate_now::on_exit(os::exit exit, const char* string, void*)
{

}
}
}