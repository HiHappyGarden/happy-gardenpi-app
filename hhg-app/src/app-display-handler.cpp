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

#include "hhg-app/app-display-handler.hpp"
#include "hhg-app/app-main.hpp"

using namespace os;
using hhg::iface::lcd;
using hhg::iface::button;
using hhg::iface::rotary_encoder;
using hhg::iface::time;
using hhg::iface::io_source;

#include "asset/font_5x8.hpp"
#include "asset/font_8x8.hpp"
#include "asset/ic_wifi_on.hpp"
#include "asset/ic_wifi_off.hpp"
using namespace hhg::asset;

#include <time.h>

namespace hhg::app
{
inline namespace v1
{
namespace
{

constexpr char APP_TAG[] = "APP DISPLAY HANDLER";

}

using write_mode = lcd::write_mode;

app_display_handler::app_display_handler(const iface::lcd::ptr &lcd
                                         , const iface::rotary_encoder::ptr &rotary_encoder
                                         , const iface::button::ptr &button
                                         , const hhg::iface::time::ptr &time
                                         , const hhg::iface::wifi::ptr& wifi
                                         , const hhg::app::app_main& app_main
                                         , hhg::app::app_data& app_data
                                         , hhg::app::app_config& app_config
                                         , const hhg::app::app_parser &app_parser
                                         )
        : lcd(lcd)
        , rotary_encoder(rotary_encoder)
        , button(button)
        , time(time)
        , wifi(wifi)
        , app_main(app_main)
        , app_data(app_data)
        , app_config(app_config)
        , app_parser(app_parser)
        , app_display_menu(*this, app_parser, app_data, app_config)
{

}

os::exit app_display_handler::init(os::error **error) OSAL_NOEXCEPT
{
    if(singleton)
    {
        if(error)
        {
            *error = OSAL_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
            OSAL_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    singleton = this;

    button->set_on_button_click(this, &button::event::on_button_click);
    rotary_encoder->set_on_rotary_encoder_event(this, &rotary_encoder::event::on_rotary_encoder_event);

    thread.create();

    return os::exit::OK;
}

void app_display_handler::on_button_click(button::status status) OSAL_NOEXCEPT
{
    //NOTE: NO log in callback!!!!
    if(is_turn_on() && !locked)
    {
        app_display_menu.button_click(status);
    }
    display_turn_off_timer = app_display_handler::DISPLAY_TURN_ODD_TIMEOUT;
}

void app_display_handler::on_rotary_encoder_event(bool ccw, bool cw, bool click) OSAL_NOEXCEPT
{
    //NOTE: NO log in callback!!!!
    if(is_turn_on() && !locked)
    {
        if(ccw)
        {
            app_display_menu.rotary_encoder_ccw();
        }

        if(cw)
        {
            app_display_menu.rotary_encoder_cw();
        }

        if(click)
        {
            app_display_menu.rotary_encoder_click();
        }
    }
    display_turn_off_timer = app_display_handler::DISPLAY_TURN_ODD_TIMEOUT;
}

void app_display_handler::paint_header(bool wifi, time_t timestamp, int16_t timezone, bool daylight_saving_time) const OSAL_NOEXCEPT
{
    lock_guard lg(mx);
    auto &&[display_width, display_height] = lcd->get_size();

    uint16_t ic_wifi_width = 0;
    uint16_t ic_wifi_height = 0;
    const uint8_t *ic_wifi = nullptr;
    uint8_t ic_wifi_size = 0;

    if(wifi)
    {
        ic_wifi_width = ic_wifi_on_width;
        ic_wifi_height = ic_wifi_on_height;
        ic_wifi = ic_wifi_on;
        ic_wifi_size = sizeof ic_wifi_on;
    }
    else
    {
        ic_wifi_width = ic_wifi_off_width;
        ic_wifi_height = ic_wifi_off_height;
        ic_wifi = ic_wifi_off;
        ic_wifi_size = sizeof ic_wifi_off;
    }
    lcd->set_rect(0, 0, display_width, 10, write_mode::REMOVE);

    lcd->set_bitmap_image(3, 0, ic_wifi_width, ic_wifi_height, ic_wifi, ic_wifi_size);

    lcd->set_rect(0, 11, display_width, 1, lcd::write_mode::ADD);

    if(timestamp > app_main::TIMESTAMP_2020)
    {
        auto&& now = time->to_string(timestamp, time::FORMAT, timezone, daylight_saving_time);

        lcd->set_str(now.c_str(), display_width - (now.length() * 5) - 5, 1, font_5x8, sizeof(font_5x8));
    }

}

pair<uint8_t, uint8_t> app_display_handler::get_font_range(app_display_handler::font font) OSAL_NOEXCEPT
{
    switch(font)
    {
        case font::F5X8:
            return {lcd::ASCII_TABLE_START_AT_IDX, font_5x8_size - 1 + lcd::ASCII_TABLE_START_AT_IDX};
        case font::F8X8:
            return {lcd::ASCII_TABLE_START_AT_IDX, font_8x8_size - 1 + lcd::ASCII_TABLE_START_AT_IDX};
    }
    return {0,0};
}

void app_display_handler::send_buffer() OSAL_NOEXCEPT
{
    lock_guard lg(mx);
    lcd->send_buffer();
}


void app_display_handler::paint_str(bool internal, const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x) const OSAL_NOEXCEPT
{
    if(!internal && (app_parser.get_source() == iface::io_source::UART || app_parser.get_source() == iface::io_source::WIFI))
    {
        return;
    }
    if(str == nullptr || strlen(str) == 0)
    {
        return;
    }
    os::lock_guard lg(mx);

    auto &&[display_width, display_height] = lcd->get_size();

    const uint8_t *font_ref = nullptr;
    uint32_t font_ref_size = 0;
    uint16_t x = 0;
    uint8_t width = 0;

    switch(font)
    {
        case font::F5X8:
            font_ref = font_5x8;
            font_ref_size = sizeof font_5x8;
            break;
        case font::F8X8:
            font_ref = font_8x8;
            font_ref_size = sizeof font_8x8;
            break;
    }
    if(font_ref == nullptr)
    {
        return;
    }

    width = font_ref[0] * strlen(str);

    switch(valign)
    {
        case valign::LEFT:
            x = 0;
            break;
        case valign::CENTER:
            x = (display_width - width) / 2;
            break;
        case valign::RIGHT:
            x = display_width - width - 2;
            break;
    }

    lcd->set_str(str, x + offset_x, y, font_ref, font_ref_size);
}

void app_display_handler::paint_char(bool internal, char c, uint16_t x, uint16_t y, app_display_handler::font font) const OSAL_NOEXCEPT
{
    if(!internal && (app_parser.get_source() == iface::io_source::UART || app_parser.get_source() == iface::io_source::WIFI))
    {
        return;
    }
    os::lock_guard lg(mx);

    const uint8_t *font_ref = nullptr;
    uint32_t font_ref_size = 0;

    switch(font)
    {
        case font::F5X8:
            font_ref = font_5x8;
            font_ref_size = sizeof font_5x8;
            break;
        case font::F8X8:
            font_ref = font_8x8;
            font_ref_size = sizeof font_8x8;
            break;
    }
    if(font_ref == nullptr)
    {
        return;
    }

    lcd->set_char(c, x, y, font_ref, font_ref_size);
}


void app_display_handler::clean(bool internal) const OSAL_NOEXCEPT
{
    if(!internal && (app_parser.get_source() == iface::io_source::UART || app_parser.get_source() == iface::io_source::WIFI))
    {
        return;
    }
    auto &&[display_width, display_height] = lcd->get_size();

    lcd->set_rect(0, 12, display_width, display_height - 12, write_mode::REMOVE);
}

auto app_display_handler::handler(void *) OSAL_NOEXCEPT -> void *
{

    if(singleton == nullptr)
    {
        return nullptr;
    }

    singleton->now_in_millis = singleton->time->get_timestamp(0, false, nullptr) * app_main::ONE_SEC_IN_MILLIS;

    uint32_t fsm_last_state = 0;

    singleton->paint_header(false);
    singleton->paint_str("Loading...");
    singleton->send_buffer();

    while(singleton->run)
    {
        auto fsm_state = singleton->app_main.get_state();
        auto [wifi_connected, wifi_has_ip] = singleton->wifi->is_connected();

        if(!singleton->locked)
        {
            if(fsm_state & app_main::ERROR)
            {
                if(fsm_last_state == fsm_state)
                {
                    goto continue_to_end_loop;
                }
                fsm_last_state = fsm_state;

            }
            else if(fsm_state & app_main::EXECUTE_ZONE)
            {
                if(fsm_last_state == fsm_state)
                {
                    goto continue_to_end_loop;
                }
                fsm_last_state = fsm_state;
            }
            else if(fsm_state & app_main::READY)
            {

                if(singleton->generic_timer == 0)
                {
                    if(singleton->display_turn_off_timer)
                    {
                        singleton->lcd->turn_on();
                    }

                    if(singleton->lcd->is_turn_on())
                    {
                        if(!singleton->app_display_menu.is_opened())
                        {
                            singleton->generic_timer = app_main::ONE_SEC_IN_MILLIS;
                            singleton->paint_header(wifi_connected && wifi_has_ip, singleton->now_in_millis / app_main::ONE_SEC_IN_MILLIS);
                            singleton->clean();
                            singleton->paint_str("Ready");
                            singleton->send_buffer();
                        }
                        else
                        {
                            auto [update_paint_header, update_send_buffer] = singleton->app_display_menu.paint();

                            if(update_paint_header)
                            {
                                singleton->paint_header(wifi_connected && wifi_has_ip, singleton->now_in_millis / app_main::ONE_SEC_IN_MILLIS);
                            }

                            if(update_send_buffer && singleton->app_display_menu.is_opened())
                            {
                                singleton->send_buffer();
                            }
                        }
                    }

                }
                else
                {
                    singleton->generic_timer -= FSM_SLEEP;
                }

                if(singleton->display_turn_off_timer == 0)
                {
                    if(singleton->app_display_menu.is_opened())
                    {
                        singleton->app_display_menu.exit();
                    }
                    singleton->lcd->turn_off();
                }

            }
            else if(fsm_state & app_main::SINCH_TIMESTAMP)
            {
                singleton->now_in_millis = singleton->time->get_timestamp(singleton->app_config.get_timezone(), singleton->app_config.get_daylight_saving_time(), nullptr) * app_main::ONE_SEC_IN_MILLIS;
            }
            else if(fsm_state & app_main::CHECK_TIMESTAMP)
            {
                if(fsm_last_state == fsm_state)
                {
                    goto continue_to_end_loop;
                }
                fsm_last_state = fsm_state;
            }
            else if(fsm_state & app_main::CHECK_WIFI_WAIT_CONNECTION)
            {
                if(fsm_last_state == fsm_state)
                {
                    goto continue_to_end_loop;
                }
                fsm_last_state = fsm_state;

                singleton->paint_header(false, 0);
                singleton->clean();

                if(wifi_connected && !wifi_has_ip)
                {
                    singleton->paint_str("Waiting IP...");
                }
                else
                {
                    singleton->paint_str("In connection...");
                }

                singleton->send_buffer();

            }
            else if(fsm_state & app_main::CHECK_USERS)
            {
                if(fsm_last_state == fsm_state)
                {
                    goto continue_to_end_loop;
                }
                fsm_last_state = fsm_state;
            }
        }
        else
        {
            singleton->display_turn_off_timer = app_display_handler::DISPLAY_TURN_ODD_TIMEOUT;
            if(singleton->generic_timer == 0)
            {
                singleton->lcd->turn_on();
                singleton->paint_header(wifi_connected && wifi_has_ip, singleton->now_in_millis / app_main::ONE_SEC_IN_MILLIS);
                handle_locked_blink_show(singleton);
                singleton->send_buffer();

                singleton->generic_timer = BLINK_SLEEP;
            }
            else
            {
                singleton->generic_timer -= FSM_SLEEP;
            }


        }

        continue_to_end_loop:
        if(singleton->display_turn_off_timer)
        {
            singleton->display_turn_off_timer -= FSM_SLEEP;
        }
        singleton->now_in_millis += FSM_SLEEP;
        us_sleep(ms_to_us(FSM_SLEEP));

    }


    singleton->thread.exit();

    return nullptr;
}

//os::exit app_display_handler::send_cmd(const os::string<128>& cmd) const OSAL_NOEXCEPT
//{
//    os::string<app_parser::RET_SIZE> ret;
//    if(app_parser.set_cmd(io_source::DISPLAY, reinterpret_cast<const uint8_t *>(cmd.c_str()), cmd.length(), ret) == osal::exit::OK)
//    {
//        return exit::OK;
//    }
//
//    return exit::KO;
//}

void app_display_handler::lock() OSAL_NOEXCEPT
{
    locked = true;
    generic_timer = 0;
}

inline void app_display_handler::on_logout() OSAL_NOEXCEPT
{
    locked = false;
}

void app_display_handler::handle_locked_blink_show(app_display_handler* self) OSAL_NOEXCEPT
{
    if(self->locked_blink_show)
    {

        self->clean(true);

        string<64> msg = "Device locked by: ";
        msg += self->app_parser.get_user_logged();

        self->paint_str(true, msg.c_str(), 26, valign::CENTER, font::F5X8);

        if(self->app_parser.is_user_logged())
        {
            switch(self->app_parser.get_source())
            {
                case iface::v1::io_source::UART:
                    self->paint_str(true, "from UART", 45, valign::CENTER, font::F5X8);
                    break;
                case iface::v1::io_source::WIFI:
                    self->paint_str(true, "from WIFI", 45, valign::CENTER, font::F5X8);
                    break;
                default:
                    break;
            }
        }
        self->locked_blink_show = false;
    }
    else
    {
        self->clean(true);
        self->locked_blink_show = true;
    }
}

//inline os::exit app_display_handler::transmit(const uint8_t *data, uint16_t size) const OSAL_NOEXCEPT
//{
//    return app_display_menu.transmit(data, size);
//}

}
}
