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

#include "osal/osal.hpp"

#include "hhg-iface/initializable.hpp"

struct i2c_inst;
typedef struct i2c_inst i2c_inst_t;

namespace hhg::iface
{
inline namespace v1
{
    struct i2c : public initializable
    {
        using ptr = os::unique_ptr<hhg::iface::i2c>;

        ~i2c() OSAL_NOEXCEPT override = default;

        virtual i2c_inst const * get_i2C_reference() const OSAL_NOEXCEPT = 0;
    };

}
}
