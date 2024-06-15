/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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

namespace hhg::iface
{
inline namespace v1
{

struct button : public initializable
{

    enum class status
    {
        PRESS,
        RELEASE
    };

    struct event
    {
        using callback = void(event::*)(enum status status);

        virtual ~event() = default;
        virtual void on_button_click(enum status status) OSAL_NOEXCEPT = 0;
    };

    using ptr = os::unique_ptr<hhg::iface::button>;

    ~button() override = default;

    virtual void set_on_button_click(event* obj, event::callback callback) OSAL_NOEXCEPT = 0;
};




}
}
