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

#include "hhg-app/app-display-handler.hpp"
using hhg::iface::button;
using hhg::iface::rotary_encoder;


namespace hhg::app
{
inline namespace v1
{

    app_display_handler::app_display_handler(const iface::lcd::ptr &lcd, const iface::rotary_encoder::ptr &rotary_encoder, const iface::button::ptr &button)
    : lcd(lcd)
    , rotary_encoder(rotary_encoder)
    , button(button)
    {

    }

    os::exit app_display_handler::init(os::error **error)
    {

        button->set_on_button_click(this, &button::event::on_button_click);
        rotary_encoder->set_on_rotary_encoder_event(this, &rotary_encoder::event::on_rotary_encoder_event);

        return os::exit::OK;
    }

    void app_display_handler::on_button_click(iface::button::status status)
    {

    }

    void app_display_handler::on_rotary_encoder_event(bool ccw, bool cw, bool click)
    {

    }


}
}
