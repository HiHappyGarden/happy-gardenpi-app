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
 * along with this program.  IfNot, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#include "hhg-platform/hardware.hpp"
#include "hhg-platform/types.hpp"
#include "hhg-platform/button.hpp"
#include "hhg-platform/lcd.hpp"
#include "hhg-platform/led.hpp"
#include "hhg-platform/releay.hpp"
#include "hhg-platform/rtc.hpp"
#include "hhg-platform/data.hpp"
#include "osal/osal.hpp"
#include "errors.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#define REGISTER_APP _IOW('a','a',uint32_t*)

namespace hhg::platform
{
inline namespace v1
{

using namespace hhg::platform;

namespace
{
hardware* me = nullptr;
constexpr inline const char APP_TAG[] = "HARDWARE";
}

void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT
{
    if (n == SIGETX)
    {
        enum type type{static_cast<uint8_t>(info->si_int)};

        os::set_check_main_loop(info->si_int);
        switch (type)
        {
        case type::BUTTON_NEXT:
            OS_LOG_DEBUG(APP_TAG, "Handled BUTTON_NEXT");

            if(me && me->button_next && dynamic_cast<button*>(me->button_next.get())->on_click)
            {
                dynamic_cast<button*>(me->button_next.get())->on_click();
            }

            break;
        case type::BUTTON_BEFORE:
            OS_LOG_DEBUG(APP_TAG, "Handled BUTTON_BEFORE");

            if(me && me->button_before && dynamic_cast<button*>(me->button_before.get())->on_click)
            {
                dynamic_cast<button*>(me->button_before.get())->on_click();
            }

            break;
        default:
            OS_LOG_WARNING(APP_TAG, "No handled signal value =  %u\n", info->si_int);
            break;
        }
    }
}

hardware::hardware() OS_NOEXCEPT
{
    me = this;
}

hardware::~hardware() OS_NOEXCEPT
{
    if(fd >= 0)
    {
        close(fd);
    }
}

bool hardware::init(error **error) OS_NOEXCEPT
{
    int32_t number = 0;
    struct sigaction act{0};

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
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::HARDWARE_NO_DRIVER), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if (ioctl(fd, REGISTER_APP, &number))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::HARDWARE_REGISTRATION), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        close(fd);
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init button_next");
    button_next = new button(fd, type::BUTTON_NEXT);
    if(button_next.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for button_next", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!button_next->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init button_next", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init button_before");
    button_before = new button(fd, type::BUTTON_BEFORE);
    if(button_before.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for button_before", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!button_before->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init button_before", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init lcd");
    lcd = new class lcd(fd);
    if(lcd.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for lcd", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!lcd->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init lcd", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init led_green");
    led_green = new hhg::platform::led(fd, type::LED_GREEN);
    if(led_green.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for led_green", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!led_green->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init led_green", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init led_green");
    led_red = new hhg::platform::led(fd, type::LED_RED);
    if(led_red.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for led_red", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!led_red->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init led_red", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init releay");
    releay = new class releay(fd);
    if(releay.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for releay", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    if(!releay->init(error))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No init releay", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }

    OS_LOG_INFO(APP_TAG, "Init rtc");

    rtc = new class rtc;
    if(rtc.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for data", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }


    OS_LOG_INFO(APP_TAG, "Init data");
    data = new class data;
    if(data.is_null())
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("No heap for data", static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return false;
    }


    if( releay->count_output() != HHGARDEN_ZONES_SIZE)
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("zones_size != HHGARDEN_ZONES_SIZE", static_cast<uint8_t>(error_code::CONF_PARAM_MISMATCH), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
    }

    return true;
}

const os::string<128>& hardware::get_info() OS_NOEXCEPT
{
    static string<128> info;

    auto file = fopen("/etc/os-release", "r");
    if(file)
    {
        char data_file[128];

        fseek(file, 0, SEEK_END);
        ssize_t bytes = ftell(file);
        fseek(file, 0, SEEK_SET);

        fread(data_file, sizeof(data_file), 1, file);

        const char token[] = "NAME=\"";
        if(char* start =strstr(data_file, token); start)
        {
            start += strlen(token);
            if(const char* end =strstr(start, "\""); end)
            {
                for(; start < end; start++)
                {
                    info += start[0];
                }
            }
        }

        fclose(file);
    }


    /* Open the command for reading. */
    file = popen("/sbin/modinfo hhgd", "r");
    if (file == NULL)
    {
        char row[257];
        memset(row, '\0', sizeof(row));

        info += " - hhgd ";

        /* Read the output a line at a time - output it. */
        while (fgets(row, sizeof(row), file) != NULL)
        {
            const char token[] = "version:";
            if(char* start =strstr(row, token); start)
            {
                start += strlen(token);
                if(const char* end =strstr(start, "\n"); end)
                {
                    for(; start < end; start++)
                    {
                        if(start[0] != ' ')
                        {
                            info += start[0];
                        }
                    }
                }
            }
        }

        /* close */
        pclose(file);
    }

    return info;
}

const os::string<128>& hardware::get_version() OS_NOEXCEPT
{
    static string<128> version;
    version += "0.10.0";
    return version;
}

int32_t hardware::get_temperature(class os::error** error) OS_NOEXCEPT
{
    int32_t fd = open("/sys/class/thermal/thermal_zone0/temp", 0);
    if (fd == -1)
    {
        perror("/sys/class/thermal/thermal_zone0/temp");
        if(error)
        {
            *error = OS_ERROR_BUILD(strerror(errno), static_cast<uint8_t>(error_code::NO_HEAP), os::get_file_name(__FILE__), __FUNCTION__, __LINE__);
        }
        return 0;
    }


    char buff[16];
    memset(buff, '\0', sizeof(buff));
    read(fd, buff, sizeof(buff));

    int32_t ret = strtol(buff, nullptr, 10);

    close(fd);

    return ret / 1000;
}



}
}

