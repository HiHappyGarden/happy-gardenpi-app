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

#pragma once

#include "hhg-iface/initializable.hpp"


namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_display_keyboard final
{
public:
    static constexpr int8_t KEYBOARD_BUFFER_SIZE = 32;

private:
    int16_t& keyboard_idx;
    class app_display_handler& app_display_handler;

    uint8_t keyboard_position = 0;
    char keyboard_buffer[KEYBOARD_BUFFER_SIZE];

    os::pair<uint8_t, uint8_t> font_limit;
public:
    app_display_keyboard(int16_t& keyboard_idx, class app_display_handler& app_display_handler);
    OSAL_NO_COPY_NO_MOVE(app_display_keyboard)

    void button_click() OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    void paint() OSAL_NOEXCEPT; //<update paint_header, update send_buffer>

    void exit();

};

}
}