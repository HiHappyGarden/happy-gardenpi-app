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

#include "hhg-app/app-display-menu.hpp"
#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::lcd;
using hhg::iface::button;

namespace hhg::app
{
inline namespace v1
{
namespace
{

constexpr char APP_TAG[] = "APP DISPLAY MENU";

}


app_display_menu::app_display_menu(class app_display_handler& app_display_handler) OSAL_NOEXCEPT
        : app_display_handler(app_display_handler)
        , app_display_keyboard(menu_idx, app_display_handler, nullptr) //todo: da gestire la callback
{
    memset(menu_level_store, -1, MENU_LEVEL_SIZE);
}

void app_display_menu::button_click(button::status status) OSAL_NOEXCEPT
{
    if(status == button::status::RELEASE || status == button::status::LONG_CLICK)
    {
        lock_guard lg(mx);
        opened = true;

        if(menu_level_store[0] == -1)
        {
            uint8_t level = 0;
            for(int16_t value: menu_level_store)
            {
                if(value == -1)
                {
                    do_paint = true;
                    menu_level_store[level] = menu_idx;
                    menu_idx = -1;
                    break;
                }
                level++;
            }
        }
        else
        {
            do_paint = true;

            switch(menu_level_store[0])
            {
                case PLANNING:

                    break;
                case IRRIGATE_NOW:

                    break;
                case WIFI:

                    break;
                case PASSWD:
                    app_display_keyboard.button_click(status);
                    break;
            }

        }
    }
}

void app_display_menu::rotary_encoder_click() OSAL_NOEXCEPT
{
    lock_guard lg(mx);
    opened = true;
    if(menu_level_store[0] == -1)
    {

    }
    else
    {
        do_paint = true;
        switch(menu_level_store[0])
        {
            case PLANNING:

                break;
            case IRRIGATE_NOW:

                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_keyboard.rotary_encoder_click();
                break;
        }

    }
}

void app_display_menu::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    lock_guard lg(mx);
    do_paint = true;
    opened = true;

    if(menu_level_store[0] == -1)
    {
        menu_idx--;
        if(menu_idx < 0)
        {
            menu_idx = MENU_SIZE - 1;
        }
    }
    else
    {
        switch(menu_level_store[0])
        {
            case PLANNING:

                break;
            case IRRIGATE_NOW:

                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_keyboard.rotary_encoder_ccw();
                break;
        }
    }
}

void app_display_menu::rotary_encoder_cw() OSAL_NOEXCEPT
{
    lock_guard lg(mx);
    do_paint = true;
    opened = true;
    if(menu_level_store[0] == -1)
    {
        menu_idx++;
        if(menu_idx >= MENU_SIZE)
        {
            menu_idx = 0;
        }
    }
    else
    {
        switch(menu_level_store[0])
        {
            case PLANNING:

                break;
            case IRRIGATE_NOW:

                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_keyboard.rotary_encoder_cw();
                break;
        }
    }
}

pair<bool, bool> app_display_menu::paint() OSAL_NOEXCEPT //<update paint_header, update send_buffer>
{
    if(!do_paint)
    {
        return {true, false};
    }
    lock_guard lg(mx);
    do_paint = false;

    if(menu_level_store[0] == -1)
    {
        app_display_handler.clean();
        app_display_handler.paint_str(first_level_labels[menu_idx]);
        return {true, true};
    }
    else
    {

        switch(menu_level_store[0])
        {
            case PLANNING:
            {

                break;
            }
            case IRRIGATE_NOW:
            {

                break;
            }
            case WIFI:
            {
                break;
            }
            case PASSWD:
            {
                if(menu_idx == -1)
                {
                    menu_idx = 'a';
                }
                app_display_handler.paint_str(passwd_level_labels[0]);
                app_display_keyboard.paint();
                break;
            }
            default:
                break;
        }
        return {false, true};
    }

    return {true, false};
}

void app_display_menu::exit() OSAL_NOEXCEPT
{
    do_paint = false;
    opened = false;
    menu_idx = -1;
    memset(menu_level_store, -1, MENU_LEVEL_SIZE);
    app_display_keyboard.exit();
}

void app_display_menu::paint_setting() OSAL_NOEXCEPT
{

}

void app_display_menu::paint_irrigates_now() OSAL_NOEXCEPT
{

}

void app_display_menu::paint_wifi() OSAL_NOEXCEPT
{

}

void app_display_menu::paint_passwd() OSAL_NOEXCEPT
{

}


}
}