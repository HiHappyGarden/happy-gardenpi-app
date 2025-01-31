/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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
using hhg::iface::button;
using hhg::iface::event_exit;

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif

namespace hhg::app
{
inline namespace v1
{

app_display_keyboard::app_display_keyboard(enum type type, int16_t& menu_idx, class app_display_handler& app_display_handler, hhg::iface::event_exit* obj, event_exit::on_exit_callback on_exit) OSAL_NOEXCEPT
: type(type)
, menu_idx(menu_idx)
, app_display_handler(app_display_handler)
, font_limit(app_display_handler.get_font_range(app_display_handler::font::F5X8))
, display_width(app_display_handler.get_size().first)
, line_max_char(display_width / WIDTH_CHAR)
, sub_keyboard_buffer(new char[line_max_char + 1])
, obj(obj)
, on_exit(on_exit)
{
    memset(keyboard_buffer, '\0', KEYBOARD_BUFFER_SIZE + 1);
#if TEST_PASSWD == 1
    strncpy(keyboard_buffer, HHG_PASSWD, sizeof(keyboard_buffer) - 1);
    keyboard_buffer_overflow = true;
#endif
}

app_display_keyboard::~app_display_keyboard()
{
    delete[] sub_keyboard_buffer;
}


void app_display_keyboard::exit() OSAL_NOEXCEPT
{
    add_char = true;
    keyboard_position = 0;
    memset(keyboard_buffer, '\0', KEYBOARD_BUFFER_SIZE + 1);
    keyboard_buffer_overflow = false;
    memset(sub_keyboard_buffer, '\0', line_max_char + 1);
    app_display_handler.paint_clean(0, app_display_handler::ROW_2_Y_OFFSET, display_width, 8);
}

void app_display_keyboard::set_first_char() OSAL_NOEXCEPT
{
    switch(type)
    {
        case type::DEFAULT:
            menu_idx = 'a';
            break;
        case type::NUMERICS:
            menu_idx = number_limit.first;
            break;
    }
}

void app_display_keyboard::button_click(button::status status) OSAL_NOEXCEPT
{
    switch(type)
    {
        case type::DEFAULT:
            if(status == button::status::RELEASE)
            {
                if(keyboard_position < KEYBOARD_BUFFER_SIZE - 1)
                {
                    keyboard_buffer[keyboard_position] = menu_idx;
                    keyboard_position++;
                    menu_idx = 'a';
                    add_char = true;
                }
            }
            else if(status == button::status::LONG_PRESS && obj && on_exit)
            {
                (obj->*on_exit)(exit::KO, keyboard_buffer, nullptr);
                exit();
            }
            break;
        case type::NUMERICS:
            if(status == button::status::RELEASE)
            {
                if(menu_idx < number_limit.first)
                {
                    menu_idx = number_limit.first;
                }

                add_char = false;
            }
            else if(status == button::status::LONG_PRESS && obj && on_exit)
            {
                (obj->*on_exit)(exit::KO, keyboard_buffer, nullptr);
                exit();
            }
            break;
    }
}

void app_display_keyboard::rotary_encoder_click() OSAL_NOEXCEPT
{
    switch(type)
    {
        case type::DEFAULT:
            if(keyboard_position > 0)
            {
                keyboard_position--;
                keyboard_buffer[keyboard_position] = '\0';
                menu_idx = 'a';
                add_char = false;
            }
            else if(obj && on_exit)
            {
                (obj->*on_exit)(exit::KO, nullptr, nullptr);
                exit();
            }
            break;
        case type::NUMERICS:
            if(obj && on_exit)
            {
                (obj->*on_exit)(exit::KO, nullptr, nullptr);
                exit();
            }
            break;
    }
}

void app_display_keyboard::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    menu_idx--;
    switch(type)
    {
        case type::DEFAULT:
            if(menu_idx < font_limit.first)
            {
                menu_idx = int16_t(font_limit.second);
            }
            break;
        case type::NUMERICS:
            if(menu_idx < number_limit.first || menu_idx > number_limit.first)
            {
                menu_idx = number_limit.second;
            }
            break;
    }
}

void app_display_keyboard::rotary_encoder_cw() OSAL_NOEXCEPT
{
    menu_idx++;
    switch(type)
    {
        case type::DEFAULT:
            if(menu_idx > font_limit.second)
            {
                menu_idx = font_limit.first;
            }
            break;
        case type::NUMERICS:
            if(menu_idx > number_limit.second || menu_idx < number_limit.first)
            {
                menu_idx = number_limit.first;
            }

            break;
    }

}

void app_display_keyboard::paint() OSAL_NOEXCEPT
{
    if(type == type::NUMERICS)
    {
        if(menu_idx < number_limit.first)
        {
            menu_idx = number_limit.first;
        }
        else if(menu_idx > number_limit.second)
        {
            menu_idx = number_limit.second;
        }
        snprintf(keyboard_buffer, sizeof(keyboard_buffer) - 1, "%u", menu_idx);
        keyboard_position = strlen(keyboard_buffer);

        app_display_handler.paint_clean(0, app_display_handler::ROW_2_Y_OFFSET, display_width, 8);
        app_display_handler.paint_str(keyboard_buffer, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::valign::CENTER, app_display_handler::font::F8X8);
        return;
    }


    if (keyboard_position < line_max_char - 1)
    {
        uint16_t x = 2 + (keyboard_position * WIDTH_CHAR);
        if(keyboard_buffer_overflow)
        {
            app_display_handler.paint_clean(0, app_display_handler::ROW_2_Y_OFFSET, display_width, 8);
            app_display_handler.paint_str(keyboard_buffer, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::valign::LEFT, app_display_handler::font::F8X8, 2);
            add_char = true;
            app_display_handler.paint_char(menu_idx , x, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::font::F8X8);
        }
        else
        {
            if(add_char)
            {
                app_display_handler.paint_char(menu_idx , x, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::font::F8X8);
            }
            else
            {
                app_display_handler.paint_clean(x, app_display_handler::ROW_2_Y_OFFSET, 8 + 8, 8);
                add_char = true;
                app_display_handler.paint_char(menu_idx , x, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::font::F8X8);
            }
        }
        keyboard_buffer_overflow = false;
    }
    else
    {
        keyboard_buffer_overflow = true;
        uint16_t delta = keyboard_position - (line_max_char - 1);
        memset(sub_keyboard_buffer, '\0', line_max_char);
        strncpy(sub_keyboard_buffer, keyboard_buffer + 3 + delta, keyboard_position - 3 - delta);

        app_display_handler.paint_clean(0, app_display_handler::ROW_2_Y_OFFSET, display_width, 8);
        app_display_handler.paint_str(sub_keyboard_buffer, app_display_handler::ROW_2_Y_OFFSET, app_display_handler::valign::LEFT, app_display_handler::font::F8X8, 3 * WIDTH_CHAR);
        app_display_handler.paint_char(menu_idx ,  ( (line_max_char - 1) * WIDTH_CHAR), app_display_handler::ROW_2_Y_OFFSET, app_display_handler::font::F8X8);

    }
}

}
}