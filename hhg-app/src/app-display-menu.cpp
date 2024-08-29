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
#include "hhg-app/app-display-menu.hpp"
#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::lcd;
using hhg::iface::button;
using hhg::iface::event_exit;

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif

namespace hhg::app
{
inline namespace v1
{
namespace
{

constexpr char APP_TAG[] = "APP DISPLAY MENU";

}


app_display_menu::app_display_menu(class app_display_handler& app_display_handler, const hhg::app::app_parser& app_parser, hhg::app::app_data& app_data, hhg::app::app_config& app_config) OSAL_NOEXCEPT
        : app_display_handler(app_display_handler)
        , app_display_passwd(app_display_handler, app_parser, menu_idx, this, &event_exit::on_exit)
        , app_display_irrigate_now(app_display_handler, app_parser, app_data, menu_idx, this, &event_exit::on_exit)
        , app_parser(app_parser)
{
    memset(menu_level_store, -1, MENU_LEVEL_SIZE);
}

void app_display_menu::button_click(button::status status) OSAL_NOEXCEPT
{
    if(status == button::status::RELEASE || status == button::status::LONG_PRESS)
    {
        lock_guard lg(mx);
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
                    app_display_irrigate_now.button_click(status);
                    break;
                case WIFI:

                    break;
                case PASSWD:
                    app_display_passwd.button_click(status);
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
                app_display_irrigate_now.rotary_encoder_click();
                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_passwd.rotary_encoder_click();
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
                app_display_irrigate_now.rotary_encoder_ccw();
                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_passwd.rotary_encoder_ccw();
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
                app_display_irrigate_now.rotary_encoder_cw();
                break;
            case WIFI:

                break;
            case PASSWD:
                app_display_passwd.rotary_encoder_cw();
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
                app_display_irrigate_now.paint();
                break;
            }
            case WIFI:
            {
                break;
            }
            case PASSWD:
            {
                app_display_passwd.paint();
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
    app_display_passwd.exit();
}

void app_display_menu::on_exit(os::exit exit, const char* string, void* args) OSAL_NOEXCEPT
{
    switch(menu_level_store[0])
    {
        case PLANNING:

            break;
        case IRRIGATE_NOW:
            //todo: gestire le action
            break;
        case WIFI:

            break;
        case PASSWD:
            if(exit == exit::KO)
            {
                menu_idx = PLANNING;
                menu_level_store[0] = -1;
            }
            else
            {

                if(!app_parser.is_user_logged())
                {
                    last_cmd = "$AUTH " HHG_USER " ";
                    last_cmd += string;
                    last_cmd += "\r\n";

                    app_display_handler.send_cmd(last_cmd);
                }
                else
                {
                    last_cmd = "$CONF 11 1 " HHG_USER " ";
                    last_cmd += string;
                    last_cmd += "\r\n";

                    app_display_handler.send_cmd(last_cmd);
                }

            }
            break;
    }
}

os::exit app_display_menu::transmit(const uint8_t* data, uint16_t size) const OSAL_NOEXCEPT
{
    char* ret = new char[size - 1];
    if(ret == nullptr)
    {
        return exit::KO;
    }

    memcpy(ret, data, size);
    switch(menu_level_store[0])
    {
        case PLANNING:

            break;
        case IRRIGATE_NOW:
            //todo: gestire le action
            break;
        case WIFI:

            break;
        case PASSWD:
            if(last_cmd.start_with("$CONF 11 1 "))
            {
                menu_idx = PLANNING;
                menu_level_store[0] = -1;
            }
            else
            {
                if(strncmp(ret, "OK", size - 1) == 0)
                {
                    menu_idx = 'a';
                    menu_level_store[0] = PASSWD;
                    app_display_handler.clean();
                }
                else
                {
                    menu_idx = PLANNING;
                    menu_level_store[0] = -1;
                }
            }
            break;
    }


    delete[] ret;
    return exit::OK;
}

}
}