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

#include "hhg-app/appmain.hpp"
#include "hhg-app/appmainfsm.hpp"
#include "hhg-intf/hardware.hpp"
#include "osal/osal.hpp"
#include "errors.hpp"

#include <unistd.h>
#include <time.h>

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "APP MAIN";

app_main* me = nullptr;

}

void on_click_button_next()
{

}
bool check_error_counter(enum app_main::state& state, uint8_t error_max, error **error);
void on_click_button_before()
{


}



app_main::app_main(intf::hardware& hardware) OS_NOEXCEPT
    : hardware(hardware)
    , app_data(hardware)
    , app_main_fsm(new class app_main_fsm(fsm, hardware))
{
    me = this;
}

app_main::~app_main() OS_NOEXCEPT
{

}

bool app_main::init(error** error) OS_NOEXCEPT
{
    if(already_instanced)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(*error, "It's permit only one instance", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }
    already_instanced = true;

    lcd_msg.clear();
    if(hardware.get_lcd()->set_text(lcd_msg, error); *error != nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(*error, "Lcd set_text() fail", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    me->hardware.get_button_next()->set_on_click(on_click_button_next);
    me->hardware.get_button_before()->set_on_click(on_click_button_before);

    OS_LOG_INFO(APP_TAG, "Load data and conf");
    if(!app_data.load(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(*error, "Lcd load() fail", static_cast<uint8_t>(error_code::INIT), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }




    return true;
}

bool app_main::fsm_start(error** error) OS_NOEXCEPT
{
    fsm_thread.create(nullptr, error);
    if(error && *error)
    {
        fsm.run = false;
        return false;
    }

    fsm.run = true;
    return true;
}

void *fsm_thread_fn(void *arg)
{
    while (me->fsm.run)
    {

        time_t current_time = me->hardware.get_rtc()->get_time();

        switch (me->fsm.state)
        {
            case app_main::INIT:
            {
                OS_LOG_INFO(APP_TAG, "fsm INIT");
                me->fsm.events.clear(app_main::ALL);

                me->fsm.old_state = me->fsm.state;
                me->fsm.state = app_main::READ_HW;

                os::error* error= nullptr;
                if(!me->app_main_fsm->init(&error))
                {
                    if(error)
                    {
                        delete error;
                        error = nullptr;

                    }
                    continue;
                }

                me->fsm.events.set(app_main::INIT);
                break;
            }
            case app_main::READ_HW:
            {
                OS_LOG_INFO(APP_TAG, "fsm READ_HW");
                me->fsm.events.clear(app_main::READ_HW);

                me->fsm.old_state = me->fsm.state;
                me->fsm.state = app_main::CHECK_DATA;

                os::error* error= nullptr;
                if(!me->app_main_fsm->read_hw(&error))
                {
                    if(error)
                    {
                        delete error;
                        error = nullptr;

                    }
                    continue;
                }

                me->fsm.events.set(app_main::READ_HW);
                break;
            }
            case app_main::CHECK_DATA:
            {
                OS_LOG_INFO(APP_TAG, "fsm CHECK_DATA");
                me->fsm.events.clear(app_main::CHECK_DATA);

                //todo check if there some data to process, add/remoce zone ecc..

                me->fsm.old_state = me->fsm.state;
                me->fsm.state = app_main::MAIN;
                me->fsm.events.set(app_main::CHECK_DATA);
                break;
            }
            case app_main::MAIN:
            {
                OS_LOG_INFO(APP_TAG, "fsm MAIN");


                tick_sleep(HHGARDEN_FSM_MAIN_SLEEP * 10);
                me->fsm.events.set(app_main::MAIN);
                break;
            }
            case app_main::START_ZONE:
            {
                OS_LOG_INFO(APP_TAG, "fsm START_ZONE");
                break;
            }
            case app_main::STOP_ZONE:
            {
                OS_LOG_INFO(APP_TAG, "fsm STOP_ZONE");
                break;
            }
            case app_main::RESET:
            {
                OS_LOG_INFO(APP_TAG, "fsm RESET");
                break;
            }
            default:
                OS_LOG_FATAL(APP_TAG, "Un handled status");
                me->fsm.run = false;
                stop_main_loop();
                break;
        }

        tick_sleep(HHGARDEN_FSM_MAIN_SLEEP);
    }

    app_main::already_instanced = false;
    return nullptr;
}

}
}

