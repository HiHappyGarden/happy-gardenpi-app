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

#include "hhg-app/app-display-irrigate-now.hpp"
#include "hhg-app/app-display-handler.hpp"
#include "hhg-app/app-parser.hpp"
using namespace os;
using hhg::iface::button;
using hhg::iface::event_exit;

namespace hhg::app
{
inline namespace v1
{


app_display_irrigate_now::app_display_irrigate_now(class app_display_handler& app_display_handler, const class app_parser& app_parser, class app_data& app_data, int16_t& menu_idx, hhg::iface::event_exit* obj, hhg::iface::event_exit::on_exit_callback on_exit)
: app_display_handler(app_display_handler)
, app_parser(app_parser)
, app_data(app_data)
, menu_idx(menu_idx)
, obj(obj)
, on_exit_callback(on_exit)
, app_display_keyboard(app_display_keyboard::type::NUMERICS, menu_idx, app_display_handler, this, on_exit)
{

}

void app_display_irrigate_now::button_click(hhg::iface::button::status status) OSAL_NOEXCEPT
{
    switch(step)
    {
        case step::NONE:
            app_display_keyboard.set_number_limit({1, HHG_SCHEDULES_SIZE});
            step = step::SCHEDULE;
            break;
        case step::SCHEDULE:
        {
            selections.schedule_idx = menu_idx - 1;
            step = step::ZONE;
            menu_idx = 0;

            auto schedule = app_data.get_data(selections.schedule_idx);
            if(schedule->zones_len)
            {
                app_display_keyboard.set_number_limit({1, schedule->zones_len});
            }

            break;
        }
        case step::ZONE:
        {
            auto&& [zones_len, zone] = app_data.get_data(selections.schedule_idx, menu_idx - 1);
            if(zones_len)
            {

                step = step::IRRIGATING;
                selections.zone_idx = menu_idx - 1;
                menu_idx = 0;
                app_display_keyboard.set_number_limit({1, 255});
            }
            else
            {
                menu_idx = 0;
            }

            //test!!!!!!
            step = step::IRRIGATING;
            selections.zone_idx = 0;
            menu_idx = 0;
            app_display_keyboard.set_number_limit({1, 255});
            break;
        }
        case step::IRRIGATING:
            selections.irrigating_minutes = menu_idx;
            menu_idx = 0;
            if(obj && on_exit_callback)
            {
                (obj->*on_exit_callback)(exit::OK, nullptr, &selections);
            }
            break;
    }
    app_display_keyboard.button_click(status);
}

void app_display_irrigate_now::rotary_encoder_click() OSAL_NOEXCEPT
{
    app_display_keyboard.rotary_encoder_click();
}

void app_display_irrigate_now::rotary_encoder_ccw() OSAL_NOEXCEPT
{
    app_display_keyboard.rotary_encoder_ccw();
}

void app_display_irrigate_now::rotary_encoder_cw() OSAL_NOEXCEPT
{
    app_display_keyboard.rotary_encoder_cw();
}

void app_display_irrigate_now::paint() OSAL_NOEXCEPT
{
    app_display_handler.clean();

    switch(step)
    {
        case step::NONE:
            app_display_keyboard.set_number_limit({1, HHG_SCHEDULES_SIZE});
            step = step::SCHEDULE;
            [[__fallthrough__]];
        case step::SCHEDULE:
        {
            app_display_handler.paint_str("From schedule");
            app_display_keyboard.paint();
            break;
        }
        case step::ZONE:
        {

            auto&& [zones_len, zone]  = app_data.get_data(selections.schedule_idx, selections.zone_idx);
            if(zones_len)
            {
                app_display_handler.paint_str("Get zone");
                app_display_keyboard.paint();
            }
            else
            {
                app_display_handler.paint_str("No zone conf");
            }

            break;
        }
        case step::IRRIGATING:

            app_display_handler.paint_str("For minutes");
            app_display_keyboard.paint();
            break;
    }



}

void app_display_irrigate_now::exit() OSAL_NOEXCEPT
{
    int i = 1;
}

void app_display_irrigate_now::on_exit(os::exit exit, const char* string, void*) OSAL_NOEXCEPT
{

    switch(exit)
    {
        case exit::KO:
            (obj->*on_exit_callback)(exit::OK, string, nullptr);
            break;
        case exit::OK:
            switch(step)
            {
                case step::SCHEDULE:
                {
                    
                    break;
                }
                case step::ZONE:
                {
                    break;
                }
                case step::IRRIGATING:

                    if(obj && on_exit_callback)
                    {
                        (obj->*on_exit_callback)(exit::OK, nullptr, &selections);
                    }

                    break;
            }
            break;
    }

}


}
}