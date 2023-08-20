/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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
#include "config.h"
#include "osal/osal.hpp"
#include "hhg-app/appdata.hpp"


namespace hhg::app
{
inline namespace v1
{

constexpr const uint8_t FSM_THREAD_PRIORITY = 4;
constexpr const uint16_t FSM_THREAD_HEAP = 4'096;


using namespace os;
namespace intf = hhg::intf;

class app_main final
{

    enum state
    {
        INIT = 0x01
    };



    const intf::hardware& hardware;
    class app_data app_data;

    string<HHGARDEN_LCD_MSG_SIZE> lcd_msg;

    state   fsm_state       = INIT;
    state   fsm_old_state;
    uint8_t fsm_errors      = 0;
    event   fsm_events;
    bool   fsm_run         = true;

    thread fsm_thread{"fst", FSM_THREAD_PRIORITY, FSM_THREAD_HEAP, fsm_thread_fn};
public:
    explicit app_main(const intf::hardware& hardware) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() OS_NOEXCEPT;

    bool init(error** error) OS_NOEXCEPT;

    bool fsm_start(error** error) OS_NOEXCEPT;

private:
    static void* fsm_thread_fn(void* arg);
};

}
}

