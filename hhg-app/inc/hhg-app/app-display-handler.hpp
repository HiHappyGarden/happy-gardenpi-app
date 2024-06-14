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
#include "hhg-app/app-parser.hpp"

namespace hhg::app
{
inline namespace v1
{

    class app_display_handler final : public hhg::iface::initializable, public hhg::iface::rotary_encoder::event, public hhg::iface::button::event
    {
        const hhg::iface::lcd::ptr& lcd;
        const hhg::iface::rotary_encoder::ptr& rotary_encoder;
        const hhg::iface::button::ptr& button;
        const hhg::app::app_parser& app_parser;

        static auto blink_timer_handler(os::timer*, void*)-> void*;
        os::timer blink_timer{ os::ms_to_us(1'000), blink_timer_handler };
    public:

        enum class font
        {
            F5X8,
            F8X8,
        };

        enum class valign
        {
            LEFT,
            CENTER,
            RIGHT,
        };

        app_display_handler(const hhg::iface::lcd::ptr& lcd, const hhg::iface::rotary_encoder::ptr& rotary_encoder, const hhg::iface::button::ptr& button, const hhg::app::app_parser& app_parser) OS_NOEXCEPT;
        ~app_display_handler() override = default;
        OS_NO_COPY_NO_MOVE(app_display_handler)

        os::exit init(os::error **error) OS_NOEXCEPT override;

        void on_button_click(iface::button::status status) OS_NOEXCEPT override;

        void on_rotary_encoder_event(bool ccw, bool cw, bool click) OS_NOEXCEPT override;

        void clean() const OS_NOEXCEPT
        {
            clean(true);
        }

        void print_str(const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x = 0, int16_t offset_y = 0) const OS_NOEXCEPT
        {
            print_str(true, str, y, valign, font, offset_x, offset_y);
        }

        void print_frame(bool wifi, const os::string<32>& now) const OS_NOEXCEPT;

        void send_buffer() OS_NOEXCEPT;

    private:
        void clean(bool internal) const OS_NOEXCEPT;

        void print_str(bool internal, const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x = 0, int16_t offset_y = 0) const OS_NOEXCEPT;

    };


}
}
