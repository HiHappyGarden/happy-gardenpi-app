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
{

}

void app_display_menu::button_click(button::status status) OSAL_NOEXCEPT
{
    if(button::status::RELEASE == status)
    {
        opened = true;
    }
}

void app_display_menu::rotary_encoder_click() OSAL_NOEXCEPT
{

}

void app_display_menu::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    do_paint = true;
    opened = true;
    menu_idx--;
    if(menu_idx < 0)
    {
        menu_idx = MENU_SIZE - 1;
    }
}

void app_display_menu::rotary_encoder_cw() OSAL_NOEXCEPT
{
    do_paint = true;
    opened = true;
    menu_idx++;
    if(menu_idx >= MENU_SIZE)
    {
        menu_idx = 0;
    }
}

void app_display_menu::paint() OSAL_NOEXCEPT
{
    if(!do_paint)
    {
        return;
    }

    if(menu_level == FIRST_LEVEL)
    {
        app_display_handler.clean();
        app_display_handler.paint_str(first_level_labels[menu_idx]);
    }

    app_display_handler.send_buffer();
    do_paint = false;

}

void app_display_menu::exit() OSAL_NOEXCEPT
{
    opened = false;
}

}
}