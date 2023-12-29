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


#include "hhg-app/appmainfsm.hpp"


namespace hhg::app
{
inline namespace v1
{

namespace
{
constexpr const char APP_TAG[] = "APP MAIN FSM";
}


bool app_main_fsm::check_error_counter(error **error) OS_NOEXCEPT
{
    if(*error)
    {
        os::printf_stack_error(APP_TAG, *error);
    }
    if(fsm.errors >= HHGARDEN_FSM_ERROR_MAX)
    {
        fsm.old_state = fsm.state;
        fsm.state = app_main::RESET;
        fsm.errors = 0;
        return false;
    }
    else
    {
        fsm.state = fsm.old_state;
        fsm.errors++;
        return true;
    }
    return true;
}

bool app_main_fsm::init(error **error) OS_NOEXCEPT
{

    hardware.get_led_green()->set_status(false, error);
    if(error && !check_error_counter(error))
    {
        tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
        return false;
    }


    hardware.get_led_red()->set_status(false, error);
    if(error && !check_error_counter(error))
    {
        tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
        return false;
    }

    for(uint8_t i = 0; i < HHGARDEN_ZONES_SIZE; i++)
    {
        hardware.get_releay()->set_status(i, false, error);
        if(error && !check_error_counter(error))
        {
            tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
            return false;
        }
    }
    return true;
}

bool app_main_fsm::read_hw(error **error) OS_NOEXCEPT
{
//    if(hardware.get_led_green()->get_status(&error))
//    {
//        error = OS_ERROR_BUILD("led_green status fail", static_cast<uint8_t>(error_code::FSM_HW_CHECK), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
//    }

//    if(!check_error_counter(&error, count_error, me->fsm.state, me->fsm.old_state, HHGARDEN_FSM_ERROR_MAX))
//    {
//        tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
//    }


//    if(me->hardware.get_led_red()->get_status(&error))
//    {
//        error = OS_ERROR_BUILD("led_red status fail", static_cast<uint8_t>(error_code::FSM_HW_CHECK), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
//    }

//    if(!check_error_counter(&error, count_error, me->fsm.state, me->fsm.old_state, HHGARDEN_FSM_ERROR_MAX))
//    {
//        tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
//    }

//    for(uint8_t i = 0; i < HHGARDEN_ZONES_SIZE; i++)
//    {
//        if(me->hardware.get_releay()->get_status(i, &error))
//        {
//            error = OS_ERROR_BUILD("led_red status fail", static_cast<uint8_t>(error_code::FSM_HW_CHECK), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
//        }

//        if(!check_error_counter(&error, count_error, me->fsm.state, me->fsm.old_state, HHGARDEN_FSM_ERROR_MAX))
//        {
//            tick_sleep(HHGARDEN_FSM_ERROR_SLEEP);
//        }
//    }

    return true;
}


}
}

