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

#include "hhg-app/app-parser.hpp"
#include "hhg-app/app-display-passwd.hpp"
#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::button;
using hhg::iface::event_exit;

namespace hhg::app
{
inline namespace v1
{


app_display_passwd::app_display_passwd(class app_display_handler& app_display_handler, const hhg::app::app_parser& app_parser, int16_t& menu_idx, hhg::iface::event_exit* obj, hhg::iface::event_exit::on_exit_callback on_exit) OSAL_NOEXCEPT
: app_display_handler(app_display_handler)
, app_parser(app_parser)
, menu_idx(menu_idx)
, obj(obj)
, on_exit_callback(on_exit)
, app_display_keyboard(app_display_keyboard::type::DEFAULT, menu_idx, app_display_handler, this, on_exit)
{

}

void app_display_passwd::button_click(button::status status) OSAL_NOEXCEPT
{
    switch(status)
    {
        case button::status::RELEASE:
            last_event = status::CLICK;
            break;
        case button::status::LONG_PRESS:
            last_event = status::CONFIRM;
            break;
    }
    app_display_keyboard.button_click(status);
}

void app_display_passwd::rotary_encoder_click() OSAL_NOEXCEPT
{
    last_event = status::BACK;
    app_display_keyboard.rotary_encoder_click();

}

void app_display_passwd::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    app_display_keyboard.rotary_encoder_ccw();

}

void app_display_passwd::rotary_encoder_cw() OSAL_NOEXCEPT
{
    app_display_keyboard.rotary_encoder_cw();
}

void app_display_passwd::paint() OSAL_NOEXCEPT
{
    if(menu_idx == -1)
    {
        app_display_keyboard.set_first_char();
    }

    if(app_parser.is_user_logged())
    {
        app_display_handler.paint_str("Set passwd");
    }
    else
    {
        app_display_handler.paint_str("Access pwd");
    }
    app_display_keyboard.paint();
}

void app_display_passwd::exit() OSAL_NOEXCEPT
{
    app_display_keyboard.exit();
}

void app_display_passwd::on_exit(os::exit exit, const char* string, void* args)
{
    if(app_parser.is_user_logged())
    {
        if(obj && on_exit_callback)
        {
            switch(last_event)
            {
                case status::CONFIRM:
                    (obj->*on_exit_callback)(exit::OK, string, nullptr);
                    break;
                case status::BACK:
                    (obj->*on_exit_callback)(exit::KO, nullptr, nullptr);
                    break;
            }


        }
    }
    else
    {
        if(obj && on_exit_callback)
        {
            (obj->*on_exit_callback)(exit::OK, app_display_keyboard.get_keyboard_buffer().c_str(), nullptr);
        }
    }

}

}
}

