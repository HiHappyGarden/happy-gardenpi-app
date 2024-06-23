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
#include "hhg-app/app-parser.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_display_handler final : public hhg::iface::rotary_encoder::event, public hhg::iface::button::event, public hhg::iface::io, public hhg::app::app_parser::auth
{
    const hhg::iface::lcd::ptr& lcd;
    const hhg::iface::rotary_encoder::ptr& rotary_encoder;
    const hhg::iface::button::ptr& button;
    const hhg::iface::time::ptr& time;
    const hhg::app::app_parser& app_parser;


    const hhg::iface::io_on_receive *obj = nullptr;
    on_receive on_receive_callback = nullptr;

    static auto blink_timer_handler(os::timer *, void *) -> void *;

    os::timer blink_timer{os::ms_to_us(1'000), blink_timer_handler};
    bool blink_show = true;

    mutable osal::mutex mx;

    static inline app_display_handler *singleton = nullptr;
public:

    enum class font
    {
        F5X8, F8X8, };

    enum class valign
    {
        LEFT, CENTER, RIGHT, };

    app_display_handler(const hhg::iface::lcd::ptr &lcd, const hhg::iface::rotary_encoder::ptr &rotary_encoder, const hhg::iface::button::ptr &button, const hhg::iface::time::ptr &time, const hhg::app::app_parser &app_parser) OSAL_NOEXCEPT;

    ~app_display_handler() override = default;
    OSAL_NO_COPY_NO_MOVE(app_display_handler)

    os::exit init(os::error **error) OSAL_NOEXCEPT override;

    void on_button_click(iface::button::status status) OSAL_NOEXCEPT override;

    void on_rotary_encoder_event(bool ccw, bool cw, bool click) OSAL_NOEXCEPT override;

    void clean() const OSAL_NOEXCEPT
    {
        os::lock_guard lg(mx);
        clean(true);
    }

    void paint_str(const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x = 0) const OSAL_NOEXCEPT
    {
        os::lock_guard lg(mx);
        paint_str(true, str, y, valign, font, offset_x);
    }

    void paint_str(const char str[]) const OSAL_NOEXCEPT
    {
        os::lock_guard lg(mx);
        paint_str(true, str, 30, valign::CENTER, font::F8X8);
    }

    void paint_header(bool wifi, time_t timestamp = 0, int16_t timezone = 0, bool daylight_saving_time = false) const OSAL_NOEXCEPT;
    void paint_string() const OSAL_NOEXCEPT;

    void send_buffer() OSAL_NOEXCEPT;

    inline void set_on_receive(const iface::io_on_receive *obj, on_receive on_receive_callback) OSAL_NOEXCEPT override
    {
        this->obj = obj;
        this->on_receive_callback = on_receive_callback;
    }

    inline os::exit transmit(const uint8_t *data, uint16_t size) const OSAL_NOEXCEPT override
    {
        return os::exit::KO;
    }

    void lock() OSAL_NOEXCEPT;

    void on_logout() const OSAL_NOEXCEPT override;

private:
    void clean(bool internal) const OSAL_NOEXCEPT;

    void paint_str(bool internal, const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x = 0) const OSAL_NOEXCEPT;

};


}
}
