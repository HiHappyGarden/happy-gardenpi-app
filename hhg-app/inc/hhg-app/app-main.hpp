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
#include "hhg-driver/hardware.hpp"
#include "osal/osal.hpp"
#include "hhg-app/app-config.hpp"
#include "hhg-app/app-parser.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_main_fsm;

class app_main final : public hhg::iface::initializable
{
public:
    enum state
    {
    	NONE        = 0x00,
        INIT        = 0x01
    };

    struct fsm
    {
        enum state   state       = INIT;
        enum state   old_state 	 = NONE;
        uint8_t      errors      = 0;
        os::event        events;
        bool         run         = true;
    };



private:
    static inline bool already_instanced = false;

    const driver::hardware& hardware;
    hhg::app::app_config app_config;
    hhg::app::app_parser app_parser;


public:
    explicit app_main(driver::hardware& hardware) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() OS_NOEXCEPT;

    os::exit init(class os::error** error) OS_NOEXCEPT override;

    os::exit fsm_start(class os::error** error) OS_NOEXCEPT;


};

}
}

