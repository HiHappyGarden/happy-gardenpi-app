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

#include "hhg-app/app-display-wifi.hpp"
#include "hhg-app/app-display-passwd.hpp"
#include "hhg-app/app-display-handler.hpp"
#include "hhg-app/app-parser.hpp"
using namespace os;
using hhg::iface::button;
using hhg::iface::event_exit;

namespace hhg::app
{
inline namespace v1
{


app_display_wifi::app_display_wifi(
        class app_display_handler& app_display_handler
        , const struct app_parser& app_parser
        , class app_config& app_config
        , int16_t& menu_idx
        , hhg::iface::event_exit* obj
        , hhg::iface::event_exit::on_exit_callback on_exit
        ) OSAL_NOEXCEPT
: app_display_handler(app_display_handler)
, app_parser(app_parser)
, app_config(app_config)
, menu_idx(menu_idx)
, obj(obj)
, on_exit_callback(on_exit)
, app_display_keyboard(app_display_keyboard::type::DEFAULT, menu_idx, app_display_handler, this, on_exit)
{

}

void app_display_wifi::button_click(hhg::iface::button::status status)
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

void app_display_wifi::rotary_encoder_click()
{
    last_event = status::BACK;
    app_display_keyboard.rotary_encoder_click();
}

void app_display_wifi::rotary_encoder_ccw()
{
    app_display_keyboard.rotary_encoder_ccw();
}

void app_display_wifi::rotary_encoder_cw()
{
    app_display_keyboard.rotary_encoder_cw();
}

void app_display_wifi::paint()
{
    if(menu_idx == -1)
    {
        app_display_keyboard.set_first_char();
    }


    if(app_parser.is_user_logged())
    {
//        inline void paint_str(const char str[], uint16_t y = 30, enum valign valign = valign::CENTER, enum font font = font::F8X8, int16_t offset_x = 0) const OSAL_NOEXCEPT
//        {
//            paint_str(true, str, y, valign, font, offset_x);
//        }
        app_display_handler.paint_clean(0, 30, 132, 8);
        switch(step)
        {
            case step::SSID:
                app_display_handler.paint_str("Enter SSID");
                break;
            case step::PASSWD:
                app_display_handler.paint_str("Enter PWD");
                break;
        }
    }
    else
    {
        app_display_handler.paint_str("Access pwd");
    }
    app_display_keyboard.paint();
}

void app_display_wifi::exit() OSAL_NOEXCEPT
{
    step = step::SSID;
    ssid = "";
    app_display_keyboard.exit();
}


void app_display_wifi::on_exit(os::exit exit, const char* string, void*) OSAL_NOEXCEPT
{
    if(app_parser.is_user_logged())
    {
        if(obj && on_exit_callback)
        {
            switch(last_event)
            {
                case status::CONFIRM:
                    switch(step)
                    {
                        case step::SSID:
                            ssid = string;
                            step = step::PASSWD;
                            break;
                        case step::PASSWD:
                            passwd  = string;
                            (obj->*on_exit_callback)(exit::OK, nullptr, nullptr);
                            while(lock)
                            {
                                osal_us_sleep(1_ms);
                            }
                            step = step::PASSWD;
                            break;
                    }
                    break;
                case status::BACK:
                    (obj->*on_exit_callback)(exit, nullptr, nullptr);
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        if(obj && on_exit_callback)
        {
            (obj->*on_exit_callback)(exit, app_display_keyboard.get_keyboard_buffer().c_str(), nullptr);
        }
    }
}

}
}

