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

namespace hhg::app
{
inline namespace v1
{

    class app_display_handler final : public hhg::iface::initializable, public hhg::iface::rotary_encoder::event, public hhg::iface::button::event
    {
        const hhg::iface::lcd::ptr& lcd;
        const hhg::iface::rotary_encoder::ptr& rotary_encoder;
        const hhg::iface::button::ptr& button;
    public:

        app_display_handler(const hhg::iface::lcd::ptr& lcd, const hhg::iface::rotary_encoder::ptr& rotary_encoder, const hhg::iface::button::ptr& button) OS_NOEXCEPT;
        ~app_display_handler() override = default;
        OS_NO_COPY_NO_MOVE(app_display_handler)

        os::exit init(os::error **error) OS_NOEXCEPT override;

        void on_button_click(iface::button::status status) OS_NOEXCEPT override;

        void on_rotary_encoder_event(bool ccw, bool cw, bool click) OS_NOEXCEPT override;


    };


}
}
