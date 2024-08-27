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
#include "hhg-iface/lcd.hpp"
#include "hhg-iface/rotary-encored.hpp"
#include "hhg-iface/button.hpp"
#include "hhg-iface/time.hpp"
#include "hhg-app/app-data.hpp"
#include "hhg-app/app-config.hpp"
#include "hhg-app/app-parser.hpp"
#include "hhg-driver/os-config.hpp"
#include "hhg-app/app-display-menu.hpp"


namespace hhg::app
{
inline namespace v1
{

class app_main;

class app_display_handler final : public hhg::iface::rotary_encoder::event, public hhg::iface::button::event, public hhg::iface::io_initializable, public hhg::app::app_parser::auth
{
    static constexpr uint8_t FSM_SLEEP = 5;
    static constexpr uint16_t BLINK_SLEEP = 500;

    const hhg::iface::lcd::ptr& lcd;
    const hhg::iface::rotary_encoder::ptr& rotary_encoder;
    const hhg::iface::button::ptr& button;
    const hhg::iface::time::ptr& time;
    const hhg::app::app_main& app_main;
    const hhg::app::app_data& app_data;
    const hhg::app::app_config& app_config;
    const hhg::app::app_parser& app_parser;

    class app_display_menu app_display_menu;

    const hhg::iface::io::receive *obj = nullptr;
    on_receive on_receive_callback = nullptr;

    static auto handler(void *) -> void *;
    os::thread thread{"app_display_handler", hhg::driver::LOW, 1024, handler};
    bool run = true;

    bool locked = false;
    bool locked_blink_show = true;

    mutable os::mutex mx;
    time_t now_in_millis = 0;
    int32_t generic_timer = 0;
    volatile int32_t display_turn_off_timer = DISPLAY_TURN_ODD_TIMEOUT;

    static inline app_display_handler *singleton = nullptr;
public:
    static constexpr uint16_t const ROW_2_Y_OFFSET = 45;
    static constexpr uint16_t DISPLAY_TURN_ODD_TIMEOUT = 60 * 1'000;

    enum class font
    {
        F5X8, F8X8
    };

    enum class valign
    {
        LEFT, CENTER, RIGHT
    };

    app_display_handler(const hhg::iface::lcd::ptr& lcd,
                        const hhg::iface::rotary_encoder::ptr& rotary_encoder,
                        const hhg::iface::button::ptr& button,
                        const hhg::iface::time::ptr& time,
                        const hhg::app::app_main& app_main,
                        hhg::app::app_data& app_data,
                        hhg::app::app_config& app_config,
                        const hhg::app::app_parser& app_parser
                        ) OSAL_NOEXCEPT;

    ~app_display_handler() override = default;
    OSAL_NO_COPY_NO_MOVE(app_display_handler)

    os::exit init(os::error **error) OSAL_NOEXCEPT override;

    void on_button_click(iface::button::status status) OSAL_NOEXCEPT override;

    void on_rotary_encoder_event(bool ccw, bool cw, bool click) OSAL_NOEXCEPT override;

    inline void clean() const OSAL_NOEXCEPT
    {
        os::lock_guard lg(mx);
        clean(true);
    }

    inline void paint_str(const char str[], uint16_t y = 30, enum valign valign = valign::CENTER, enum font font = font::F8X8, int16_t offset_x = 0) const OSAL_NOEXCEPT
    {
        paint_str(true, str, y, valign, font, offset_x);
    }

    inline void paint_char(char c, uint16_t x, uint16_t y, enum font font = font::F8X8) const OSAL_NOEXCEPT
    {
        paint_char(true, c, x, y, font);
    }

    inline void paint_clean(uint16_t x, uint16_t y, uint16_t width, uint16_t height) const OSAL_NOEXCEPT
    {
        lcd->set_rect(x, y, width, height, hhg::iface::lcd::write_mode::REMOVE);
    }

    inline os::pair<uint16_t, uint16_t> get_size() const OSAL_NOEXCEPT
    {
        return lcd->get_size();;
    }

    void paint_header(bool wifi, time_t timestamp = 0, int16_t timezone = 0, bool daylight_saving_time = false) const OSAL_NOEXCEPT;

    os::pair<uint8_t, uint8_t> get_font_range(enum font font = font::F8X8);

    void send_buffer() OSAL_NOEXCEPT;

    inline void set_on_receive(const iface::io::receive *obj, on_receive on_receive_callback) OSAL_NOEXCEPT override
    {
        this->obj = obj;
        this->on_receive_callback = on_receive_callback;
    }

    os::exit send_cmd(const os::string<128>& cmd) const OSAL_NOEXCEPT;

    inline os::exit send_cmd(const os::string<128>&& cmd) const OSAL_NOEXCEPT
    {
        return send_cmd(cmd);
    }

    void lock() OSAL_NOEXCEPT;

    void on_logout() OSAL_NOEXCEPT override;

    inline bool is_turn_on() const OSAL_NOEXCEPT
    {
        return lcd->is_turn_on();
    }

private:
    void paint_str(bool internal, const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x = 0) const OSAL_NOEXCEPT;

    void paint_char(bool internal, char c, uint16_t x, uint16_t y, enum font font) const OSAL_NOEXCEPT;

    void clean(bool internal) const OSAL_NOEXCEPT;

    static void handle_locked_blink_show(app_display_handler* self) OSAL_NOEXCEPT;

    os::exit transmit(const uint8_t *data, uint16_t size) const OSAL_NOEXCEPT override;
};


}
}
