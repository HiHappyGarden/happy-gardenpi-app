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
#include "hhg-app/app-display-keyboard.hpp"


namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_display_menu final
{
    static constexpr uint8_t MENU_SIZE = 4;
    static constexpr uint8_t MENU_LABEL_SIZE = 16;
    static constexpr int8_t MENU_LEVEL_SIZE = 2;

    class app_display_handler& app_display_handler;
    class app_display_keyboard app_display_keyboard;

    enum first_level
    {
        PLANNING,
        IRRIGATE_NOW,
        WIFI,
        PASSWD
    };

    char const first_level_labels[MENU_SIZE][MENU_LABEL_SIZE] = {
            [PLANNING] = "Planning",
            [IRRIGATE_NOW] = "Irrigates now",
            [WIFI] = "WiFi",
            [PASSWD] = "Passwd"
    };

    enum passwd_level
    {
        SET_PASSWD
    };

    char const passwd_level_labels[MENU_SIZE][MENU_LABEL_SIZE] = {
            [SET_PASSWD] = "Set passwd"
    };

    os::mutex mx;

    bool do_paint = false;
    bool opened = false;

    int16_t menu_idx = -1;

    int16_t menu_level_store[MENU_LEVEL_SIZE];

public:
    explicit app_display_menu(class app_display_handler& app_display_handler) OSAL_NOEXCEPT;
    OSAL_NO_COPY_NO_MOVE(app_display_menu)

    void button_click(hhg::iface::button::status status) OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    os::pair<bool, bool> paint() OSAL_NOEXCEPT; //<update paint_header, update send_buffer>

    void exit() OSAL_NOEXCEPT;

    inline bool is_opened() const OSAL_NOEXCEPT
    {
        return opened;
    }

private:

    void paint_setting() OSAL_NOEXCEPT;
    void paint_irrigates_now() OSAL_NOEXCEPT;
    void paint_wifi() OSAL_NOEXCEPT;
    void paint_passwd() OSAL_NOEXCEPT;


};

}
}