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

#include "hhg-app/app-display-passwd.hpp"
#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::button;
using hhg::iface::event_exit;

namespace hhg::app
{
inline namespace v1
{


app_display_passwd::app_display_passwd(int16_t& menu_idx, class app_display_handler& app_display_handler, event_exit* obj, event_exit::on_exit_callback on_exit) OSAL_NOEXCEPT
: menu_idx(menu_idx)
, app_display_handler(app_display_handler)
, obj(obj)
, on_exit_callback(on_exit)
, app_display_keyboard(menu_idx, app_display_handler, this, on_exit)
, app_display_auth(menu_idx, app_display_handler, this, &app_display_auth::event_auth::on_auth)
{

}

void app_display_passwd::button_click(button::status status) OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        app_display_keyboard.button_click(status);
    }
    else
    {
        app_display_auth.button_click(status);
    }
}

void app_display_passwd::rotary_encoder_click() OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        app_display_keyboard.rotary_encoder_click();
    }
    else
    {
        app_display_auth.rotary_encoder_click();
    }
}

void app_display_passwd::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        app_display_keyboard.rotary_encoder_ccw();
    }
    else
    {
        app_display_auth.rotary_encoder_ccw();
    }
}

void app_display_passwd::rotary_encoder_cw() OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        app_display_keyboard.rotary_encoder_cw();
    }
    else
    {
        app_display_auth.rotary_encoder_cw();
    }
}

void app_display_passwd::paint() OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        if(menu_idx == -1)
        {
            app_display_keyboard.set_first_char();
        }
        app_display_handler.paint_str(submenu_label.c_str());
        app_display_keyboard.paint();
    }
    else
    {
        app_display_auth.paint();
    }

}

void app_display_passwd::exit() OSAL_NOEXCEPT
{
    if(app_display_auth.is_auth())
    {
        app_display_keyboard.exit();
    }
    else
    {
        app_display_auth.exit();
    }
}

void app_display_passwd::on_exit(os::exit exit, const char* string)
{
    if(obj && on_exit_callback)
    {
        (obj->*on_exit_callback)(exit, nullptr);
    }
}

void app_display_passwd::on_auth(bool auth)
{
    this->auth = auth;
}

}
}

