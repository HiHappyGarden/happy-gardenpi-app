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

#include "hhg-app/app-display-auth.hpp"
#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::event_exit;


namespace hhg::app
{
inline namespace v1
{

app_display_auth::app_display_auth(int16_t& menu_idx, class app_display_handler& app_display_handler, event_auth* obj, event_auth::on_auth_calback on_auth) OSAL_NOEXCEPT
: obj(obj)
, on_auth(on_auth)
{

}

void app_display_auth::button_click(hhg::iface::button::status status) OSAL_NOEXCEPT
{

}

void app_display_auth::rotary_encoder_click() OSAL_NOEXCEPT
{

}

void app_display_auth::rotary_encoder_ccw() OSAL_NOEXCEPT
{

}

void app_display_auth::rotary_encoder_cw() OSAL_NOEXCEPT
{

}

void app_display_auth::paint() OSAL_NOEXCEPT
{

}

void app_display_auth::exit() OSAL_NOEXCEPT
{

}

}
}