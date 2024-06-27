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

#include "hhg-app/app-display-keyboard.hpp"
#include "hhg-app/app-display-handler.hpp"

using namespace os;

namespace hhg::app
{
inline namespace v1
{

app_display_keyboard::app_display_keyboard(int16_t& menu_idx, class app_display_handler& app_display_handler) OSAL_NOEXCEPT
: menu_idx(menu_idx)
, app_display_handler(app_display_handler)
, font_limit(app_display_handler.get_font_range(app_display_handler::font::F5X8))
{
    memset(keyboard_buffer, '\0', KEYBOARD_BUFFER_SIZE);
}

void app_display_keyboard::exit() OSAL_NOEXCEPT
{
    keyboard_position = 0;
    memset(keyboard_buffer, '\0', KEYBOARD_BUFFER_SIZE);
}

void app_display_keyboard::button_click() OSAL_NOEXCEPT
{
    if(keyboard_position < KEYBOARD_BUFFER_SIZE - 1)
    {
        keyboard_buffer[keyboard_position] = menu_idx;
        keyboard_position++;
        menu_idx = 'a';
        add_char = true;
    }
}

void app_display_keyboard::rotary_encoder_click() OSAL_NOEXCEPT
{
    if(keyboard_position > 0)
    {
        keyboard_position--;
        keyboard_buffer[keyboard_position] = '\0';
        menu_idx = 'a';
        add_char = false;
    }
}

void app_display_keyboard::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    menu_idx--;
    if(menu_idx < font_limit.first)
    {
        menu_idx = int16_t(font_limit.second);
    }
}

void app_display_keyboard::rotary_encoder_cw() OSAL_NOEXCEPT
{
    menu_idx++;
    if(menu_idx > font_limit.second)
    {
        menu_idx = font_limit.first;
    }
}

void app_display_keyboard::paint() OSAL_NOEXCEPT
{
    static uint16_t const y = 45;
    uint8_t const width_char = 8;
    uint16_t x = 2 + (keyboard_position * width_char);

    printf("-->x:%u\n", x);

    if(add_char)
        app_display_handler.paint_char(menu_idx , x, y, app_display_handler::font::F8X8);
    else
        app_display_handler.paint_clean(x, y, 8 + 8, 8);
}

}
}