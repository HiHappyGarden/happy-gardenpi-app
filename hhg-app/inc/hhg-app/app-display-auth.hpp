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
#include "osal/osal.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_display_auth final
{
public:
    struct event_auth
    {
        using on_auth_calback = void(event_auth::*)(bool);

        virtual ~event_auth() = default;

        virtual void on_auth(bool) = 0;
    };


private:
    event_auth* obj = nullptr;
    event_auth::on_auth_calback on_auth = nullptr;

    bool auth = false;
public:
    app_display_auth(int16_t& menu_idx, class app_display_handler& app_display_handler, event_auth* obj, event_auth::on_auth_calback on_auth) OSAL_NOEXCEPT;
    OSAL_NO_COPY_NO_MOVE(app_display_auth)

    void button_click(hhg::iface::button::status status) OSAL_NOEXCEPT;

    void rotary_encoder_click() OSAL_NOEXCEPT;

    void rotary_encoder_ccw() OSAL_NOEXCEPT;

    void rotary_encoder_cw() OSAL_NOEXCEPT;

    void paint() OSAL_NOEXCEPT;

    void exit() OSAL_NOEXCEPT;

    inline bool is_auth() const OSAL_NOEXCEPT
    {
        return auth;
    }
};

}
}