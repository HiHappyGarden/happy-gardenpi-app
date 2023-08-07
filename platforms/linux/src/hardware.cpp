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
 * along with this program.  IfNot, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "hhg-platform/hardware.hpp"
#include "hhg-platform/types.hpp"
#include "hhg-platform/button.hpp"


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define REGISTER_APP _IOW('a','a',uint32_t*)

namespace hhg::platform
{
inline namespace v1
{

namespace
{
hardware* me = nullptr;
constexpr const char APP_TAG[] = "HARDWARE";
}

void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT
{
    if (n == SIGETX)
    {
        if (n == SIGETX)
        {
            hhgd_type type{info->si_int};

            os::set_check_main_loop(info->si_int);
            switch (type)
            {
            case hhgd_type::HHGD_BUTTON_NEXT:
                OS_LOG_DEBUG(APP_TAG, "Handled HHGD_BUTTON_NEXT");

                if(me && me->button_next && me->button_next->on_click)
                {
                    me->button_next->on_click();
                }

                break;
            case hhgd_type::HHGD_BUTTON_BEFORE:
                OS_LOG_DEBUG(APP_TAG, "Handled HHGD_BUTTON_BEFORE");

                if(me && me->button_before && me->button_before->on_click)
                {
                    me->button_before->on_click();
                }

                break;
            default:
                OS_LOG_WARNING(APP_TAG, "No handled signal value =  %u\n", info->si_int);
                break;
            }
        }
    }
}



hardware::hardware() OS_NOEXCEPT
{
    me = this;
}

hardware::~hardware() OS_NOEXCEPT
{
    if(button_next)
    {
        delete button_next;
    }
    if(button_before)
    {
        delete button_before;
    }

    if(fd >= 0)
    {
        close(fd);
    }
}

bool hardware::init(error **error) OS_NOEXCEPT
{
    int32_t number;
    struct sigaction act;

//    /* install custom signal handler */
//    sigemptyset (&act.sa_mask);
//    act.sa_flags = (SA_SIGINFO | SA_RESETHAND);
//    act.sa_sigaction = ctrl_c_handler;
//    sigaction (SIGINT, &act, NULL);

    /* install custom signal handler */
    sigemptyset(&act.sa_mask);
    act.sa_flags = (SA_SIGINFO | SA_RESTART);
    act.sa_sigaction = sig_event_handler;
    sigaction(SIGETX, &act, NULL);


    fd = open(HHGD_PATH, O_RDWR);
    if(fd < 0)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::HHGD_HARDWARE_NO_DRIVER), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if (ioctl(fd, REGISTER_APP, &number))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::HHGD_HARDWARE_REGISTRATION), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        close(fd);
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init button_next");
    button_next = new button(fd);
    if(button_next == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for button_next", static_cast<uint8_t>(error_code::HHGD_NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!button_next->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init button_next", static_cast<uint8_t>(error_code::HHGD_NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init button_before");
    button_before = new button(fd);
    if(button_before == nullptr)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for button_before", static_cast<uint8_t>(error_code::HHGD_NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!button_before->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init button_before", static_cast<uint8_t>(error_code::HHGD_NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }



    return true;
}


}
}

