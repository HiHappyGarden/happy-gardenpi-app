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

app_display_keyboard::app_display_keyboard(int16_t& keyboard_idx, class app_display_handler& app_display_handler) OSAL_NOEXCEPT
: keyboard_idx(keyboard_idx)
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
        keyboard_buffer[keyboard_position] = keyboard_idx;
        keyboard_position++;
        keyboard_idx = 'a';
    }
}

void app_display_keyboard::rotary_encoder_click() OSAL_NOEXCEPT
{

}

void app_display_keyboard::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    keyboard_idx--;
    if(keyboard_idx < font_limit.first)
    {
        keyboard_idx = int16_t(font_limit.second);
    }
}

void app_display_keyboard::rotary_encoder_cw() OSAL_NOEXCEPT
{
    keyboard_idx++;
    if(keyboard_idx > font_limit.second)
    {
        keyboard_idx = font_limit.first;
    }
}

void app_display_keyboard::paint() OSAL_NOEXCEPT
{
    static uint16_t const y = 45;
    uint8_t const width_char = 8;
    uint16_t x = 2 + (keyboard_position * width_char);

    app_display_handler.paint_char(keyboard_idx , x, y, app_display_handler::font::F8X8);
}

}
}