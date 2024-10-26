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
#include "hhg-iface/button.hpp"
#include "hhg-app/app-display-passwd.hpp"
#include "hhg-app/app-display-irrigate-now.hpp"
#include "hhg-app/app-display-wifi.hpp"


namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_parser;
class app_display_menu final : public hhg::iface::event_exit
{
    static constexpr uint8_t MENU_LABEL_SIZE = 16;

    enum
    {
        MENU_LEVEL_ZERO,
        MENU_LEVEL_ONE,
        MENU_LEVEL_SIZE
    };

    class app_display_handler& app_display_handler;
    class app_display_passwd app_display_passwd;
    class app_display_irrigate_now app_display_irrigate_now;
    mutable class app_display_wifi app_display_wifi;
    const hhg::app::app_parser& app_parser;

    enum first_level
    {
        IRRIGATE_NOW,
        WIFI,
        PASSWD,
        MENU_SIZE
    };

    char const first_level_labels[MENU_SIZE][MENU_LABEL_SIZE] = {
            [IRRIGATE_NOW] = "Irrigate now",
            [WIFI] = "WiFi",
            [PASSWD] = "Passwd"
    };


    os::mutex mx;

    mutable bool do_paint = false;
    bool opened = false;

    mutable int16_t menu_idx = -1;

    mutable int16_t menu_level_store[MENU_LEVEL_SIZE] = {
                [MENU_LEVEL_ZERO] = -1,
            [MENU_LEVEL_ONE] = -1,
    };
//    mutable os::string<128> last_cmd;

public:
    explicit app_display_menu(
            class app_display_handler& app_display_handler
            , const hhg::app::app_parser& app_parser
            , hhg::app::app_data& app_data
            , hhg::app::app_config& app_config
            ) OSAL_NOEXCEPT;
    ~app_display_menu() override = default;
    OSAL_NO_COPY_NO_MOVE(app_display_menu)

    void button_click(hhg::iface::button::status status) OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    os::pair<bool, bool> paint() OSAL_NOEXCEPT; //<update paint_header, update send_buffer>

    inline void set_do_paint(bool do_paint) OSAL_NOEXCEPT
    {
        this->do_paint = do_paint;
    }

    void exit() OSAL_NOEXCEPT;

    inline bool is_opened() const OSAL_NOEXCEPT
    {
        return opened;
    }

//    os::exit transmit(const uint8_t *data, uint16_t size) const OSAL_NOEXCEPT;

private:

    void on_exit(os::exit exit, const char* string, void*) OSAL_NOEXCEPT override;

};

}
}