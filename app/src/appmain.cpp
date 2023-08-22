/***************************************************************************
 *
 * Hi Happy Garden Interfaces
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
#include "hhg-intf/hardware.hpp"
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

bool check_error_counter(uint8_t& error, enum app_main::state& state, enum app_main::state& old_state, uint8_t error_max)
{
    if(error >= error_max)
    {
        old_state = state;
        state = app_main::RESET;
        error = 0;
        return false;
    }
    else
    {
        error++;
        return true;
    }
}

}

void on_click_button_next()
{

}

void on_click_button_before()
{


}



app_main::app_main(const intf::hardware& hardware) OS_NOEXCEPT
    : hardware(hardware)
    , app_data(hardware)
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
        fsm_run = false;
        return false;
    }

    fsm_run = true;
    return true;
}

void *fsm_thread_fn(void *arg)
{
    uint8_t count_error = 0;

    while (me->fsm_run)
    {

        time_t current_time = me->hardware.get_rtc()->get_time();

        switch (me->fsm_state)
        {
            case app_main::INIT:
            {
                me->fsm_events.clear(app_main::ALL);

                os::error* error= nullptr;
                count_error = 0;

                me->hardware.get_led_green()->set_status(false, &error);
                if(error)
                {
                    os::printf_stack_error(APP_TAG, error);
                    delete error;
                    error = nullptr;
                    if(!check_error_counter(count_error, me->fsm_state, me->fsm_old_state, app_main::FSM_MAX_ERROR))
                    {
                         os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP));
                    }
                    else
                    {
                         os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP / 2));
                    }
                }

                me->hardware.get_led_red()->set_status(false, &error);
                if(error)
                {
                    error++;
                    os::printf_stack_error(APP_TAG, error);
                    delete error;
                    error = nullptr;
                    if(!check_error_counter(count_error, me->fsm_state, me->fsm_old_state, app_main::FSM_MAX_ERROR))
                    {
                         os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP));
                    }
                    else
                    {
                         os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP / 2));
                    }
                }

                for(uint8_t i = 0; i < HHGARDEN_ZONES_SIZE; i++)
                {
                    me->hardware.get_releay()->set_status(i, false, &error);
                    if(error)
                    {
                        os::printf_stack_error(APP_TAG, error);
                        delete error;
                        error = nullptr;
                        if(!check_error_counter(count_error, me->fsm_state, me->fsm_old_state, app_main::FSM_MAX_ERROR))
                        {
                            os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP));
                        }
                        else
                        {
                            os::tick_from_us(ms_to_us(app_main::ERROR_SLEEP / 2));
                        }
                    }
                }


                me->fsm_old_state = me->fsm_state;
                me->fsm_state = app_main::READ_HW;
                me->fsm_events.set(app_main::INIT);
                break;
            }
            case app_main::READ_HW:
            {
                me->fsm_events.clear(app_main::READ_HW);

                me->fsm_old_state = me->fsm_state;
                me->fsm_state = app_main::CHECK_DATA;
                me->fsm_events.set(app_main::READ_HW);
                break;
            }
            case app_main::CHECK_DATA:
            {
                me->fsm_events.clear(app_main::CHECK_DATA);

                me->fsm_old_state = me->fsm_state;
                me->fsm_state = app_main::MAIN;
                me->fsm_events.set(app_main::CHECK_DATA);
                break;
            }
            case app_main::MAIN:
            {

                me->fsm_events.set(app_main::MAIN);
                break;
            }
            case app_main::START_ZONE:
            {

                break;
            }
            case app_main::STOP_ZONE:
            {

                break;
            }
            case app_main::RESET:
            {

                break;
            }
        }

        os::tick_from_us(ms_to_us(app_main::MAIN_SLEEP));
    }

    app_main::already_instanced = false;
    return nullptr;
}

}
}

