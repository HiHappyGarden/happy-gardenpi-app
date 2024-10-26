/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025  Antonio Salsi <passy.linux@zresa.it>
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

    struct rgb_led: public initializable
    {
        struct rgb final
        {
            uint8_t red = 0x00;
            uint8_t green = 0x00;
            uint8_t blue = 0x00;
            inline bool is_off() const OSAL_NOEXCEPT
            {
                return red + green + blue == 0;
            }
        };

        using ptr = os::unique_ptr<hhg::iface::rgb_led>;

        ~rgb_led() OSAL_NOEXCEPT override = default;

        virtual void set_red(uint8_t value) const OSAL_NOEXCEPT = 0;

        virtual void set_green(uint8_t value) const OSAL_NOEXCEPT = 0;

        virtual void set_blue(uint8_t value) const OSAL_NOEXCEPT = 0;

        virtual void set_rgb(uint8_t red, uint8_t green, uint8_t blue) const OSAL_NOEXCEPT = 0;

        virtual const rgb& get_rgb() const OSAL_NOEXCEPT = 0;


    };

}
}