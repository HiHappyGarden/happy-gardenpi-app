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


#include "hhg-app/app-led.hpp"
using namespace os;
using hhg::iface::rgb_led;

#define RGB_OFF() singleton->rgb_led->set_rgb(0, 0, 0)
#define RGB_LOADING() singleton->rgb_led->set_rgb(0, 0xFF, 0)
#define RGB_WARNING() singleton->rgb_led->set_rgb(0xFF, 0x5A, 0X00)
#define RGB_ERROR() singleton->rgb_led->set_rgb(0xFF, 0, 0)
#define RGB_READY() singleton->rgb_led->set_rgb(0, 0xFF, 0)

namespace hhg::app
{
inline namespace v1
{


    app_led::app_led(const rgb_led::ptr& rgb_led) OSAL_NOEXCEPT
    : rgb_led(rgb_led)
    {

    }

    app_led::~app_led() = default;

    inline os::exit app_led::init(class error **error) OSAL_NOEXCEPT
    {
        if(singleton)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
        singleton = this;

        loading();

        return thread.create(error);
    }

    void* app_led::handler(void *arg) OSAL_NOEXCEPT
    {

        int64_t timer_on = 0;
        int64_t timer_off = 0;
        bool on = true;

        while(singleton)
        {

            switch (singleton->status)
            {
                case status::RUNNING_IRRIGATION:
                case status::LOADING:
                    if(on)
                    {
                        RGB_LOADING();
                        timer_on = 5 * TICK;
                        on = true;
                    }
                    else
                    {
                        RGB_OFF();
                        timer_off = 5 * TICK;
                        on = false;
                    }
                    singleton->curren_status = status::LOADING;
                    break;
                case status::WARNING:
                    if(on)
                    {
                        RGB_WARNING();
                        timer_on = 3 * TICK;
                        on = true;
                    }
                    else
                    {
                        RGB_OFF();
                        timer_off = 3 * TICK;
                        on = false;
                    }
                    singleton->curren_status = status::WARNING;
                    break;
                case status::ERROR:
                    if(on)
                    {
                        RGB_ERROR();
                        timer_on = 2 * TICK;
                        on = true;
                    }
                    else
                    {
                        RGB_OFF();
                        timer_off = 2 * TICK;
                        on = false;
                    }
                    singleton->curren_status = status::ERROR;
                    break;
                case status::READY:
                    if(on)
                    {
                        RGB_READY();
                        timer_on = 1000 * TICK;
                        on = true;
                    }
                    else
                    {
                        timer_off = 0;
                        on = false;
                    }
                    singleton->curren_status = status::READY;
                    break;
                default:
                    break;
            }

            if(timer_on > 0)
            {
                timer_on -= TICK;
                singleton->status = status::NONE;
            }
            else if(timer_off > 0)
            {
                timer_off -= TICK;
                singleton->status = status::NONE;
            }
            else if(timer_on == 0 && timer_off == 0)
            {
                on = !on;
                singleton->status = singleton->curren_status;
            }


            us_sleep(ms_to_us(TICK));
        }



        return nullptr;
    }

    void app_led::loading() const OSAL_NOEXCEPT
    {
        if(curren_status != status::LOADING)
        {
            status = status::LOADING;
        }
    }

    void app_led::warning() const OSAL_NOEXCEPT
    {
        if(curren_status != status::WARNING)
        {
            status = status::WARNING;
        }
    }

    void app_led::error() const OSAL_NOEXCEPT
    {
        if(curren_status != status::ERROR)
        {
            status = status::ERROR;
        }
    }

    void app_led::ready() const OSAL_NOEXCEPT
    {
        if(curren_status != status::READY)
        {
            status = status::READY;
        }
    }

    void app_led::running_irrigation() const {
        if(curren_status != status::READY)
        {
            status = status::READY;
        }
    }

}



}
