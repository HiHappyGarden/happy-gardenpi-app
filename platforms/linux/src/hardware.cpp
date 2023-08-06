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

#include "hhg-platform/hardware.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <errno.h>

#define REGISTER_APP _IOW('a','a',uint32_t*)

namespace hhg::platform
{
inline namespace v1
{

namespace
{
//void ctrl_c_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT
//{
//    if (n == SIGINT)
//    {
//        printf("\nrecieved ctrl-c\n");
//    }
//}

void sig_event_handler(int n, siginfo_t *info, void *unused) OS_NOEXCEPT
{
    if (n == SIGETX)
    {
        printf ("Received signal from kernel : Value =  %u\n", info->si_int);
    }
}
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
            *error = OS_ERROR_BUILD("No find hhgd drive", static_cast<uint8_t>(error_code::HHGD_NO_DRIVER));
        }
        return false;
    }

    if (ioctl(fd, REGISTER_APP, &number))
    {
        if(error)
        {
            *error = OS_ERROR_BUILD("Not possible register ioctl", static_cast<uint8_t>(error_code::HHGD_NO_REGISTRATION));
        }
        close(fd);
        return false;
    }

    return true;
}


}
}

