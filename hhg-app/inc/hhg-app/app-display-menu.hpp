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
#include "hhg-iface/button.hpp"


namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_display_menu final
{
    class app_display_handler& app_display_handler;

    bool do_paint = false;
    bool opened = false;

    char const menu_labels[4][16] = {
            "Settings",
            "Irrigates now",
            "WiFi",
            "Passwd"
    };

    uint8_t menu_idx = 0;

public:
    explicit app_display_menu(class app_display_handler& app_display_handler) OSAL_NOEXCEPT;
    OSAL_NO_COPY_NO_MOVE(app_display_menu)

    void button_click(hhg::iface::button::status status) OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    void paint() OSAL_NOEXCEPT;

    void exit() OSAL_NOEXCEPT;

    inline bool is_opened() const OSAL_NOEXCEPT
    {
        return opened;
    }

};

}
}