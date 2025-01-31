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

#pragma once

#include "hhg-iface/initializable.hpp"
#include "hhg-iface/button.hpp"
#include "hhg-iface/event-exit.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_display_keyboard final
{
public:

    enum class type
    {
        DEFAULT,
        NUMERICS
    };

    static constexpr int8_t KEYBOARD_BUFFER_SIZE = 32;
    static constexpr uint8_t const WIDTH_CHAR = 8;

private:
    enum type type;
    int16_t& menu_idx;
    class app_display_handler& app_display_handler;

    bool add_char = true;
    uint8_t keyboard_position = 0;
    char keyboard_buffer[KEYBOARD_BUFFER_SIZE + 1];
    bool keyboard_buffer_overflow = false;

    os::pair<uint8_t, uint8_t> font_limit;
    os::pair<int16_t, int16_t> number_limit { 0, INT16_MAX};
    uint8_t const display_width;
    uint8_t const line_max_char;
    char* sub_keyboard_buffer = nullptr;

    hhg::iface::event_exit* obj = nullptr;
    hhg::iface::event_exit::on_exit_callback on_exit = nullptr;

public:
    app_display_keyboard(enum type type, int16_t& menu_idx, class app_display_handler& app_display_handler, hhg::iface::event_exit* obj, hhg::iface::event_exit::on_exit_callback on_exit);
    ~app_display_keyboard();
    OSAL_NO_COPY_NO_MOVE(app_display_keyboard)

    inline void set_number_limit(const os::pair<int16_t, int16_t>&& limit) OSAL_NOEXCEPT
    {
        number_limit = limit;
    }

    void button_click(hhg::iface::button::status status) OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    void paint() OSAL_NOEXCEPT;

    void exit() OSAL_NOEXCEPT;

    void set_first_char() OSAL_NOEXCEPT;

    inline os::string<KEYBOARD_BUFFER_SIZE + 1> const get_keyboard_buffer() const OSAL_NOEXCEPT
    {
        return keyboard_buffer;
    }
};

}
}