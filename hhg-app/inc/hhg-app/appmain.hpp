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
#include "hhg/config.h"
#include "osal/osal.hpp"
#include "hhg-intf/hardware.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_main_fsm;

class app_main final
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

    const intf::hardware& hardware;
//    class app_data app_data;
//    struct fsm fsm;
//    unique_ptr<class app_main_fsm> app_main_fsm;

//    string<HHGARDEN_LCD_MSG_SIZE> lcd_msg;




//    thread fsm_thread{"fsm thread", HHGARDEN_FSM_THREAD_PRIORITY, HHGARDEN_FSM_THREAD_HEAP, fsm_thread_fn};
//
//    friend void* fsm_thread_fn(void* arg);
//    friend void on_click_button_next();
//    friend void on_click_button_before();
public:
    explicit app_main(intf::hardware& hardware) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() OS_NOEXCEPT;

    os::exit init(class error** error) OS_NOEXCEPT;

    os::exit fsm_start(class error** error) OS_NOEXCEPT;


};

}
}

