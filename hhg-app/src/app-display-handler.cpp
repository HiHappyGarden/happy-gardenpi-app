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

using namespace os;
using hhg::iface::lcd;
using hhg::iface::button;
using hhg::iface::rotary_encoder;

#include "assets/font_5x8.hpp"
#include "assets/font_8x8.hpp"
#include "assets/ic_wifi_on.hpp"
#include "assets/ic_wifi_off.hpp"

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

app_display_handler::app_display_handler(const iface::lcd::ptr &lcd, const iface::rotary_encoder::ptr &rotary_encoder, const iface::button::ptr &button, const hhg::app::app_parser &app_parser)
        : lcd(lcd)
        , rotary_encoder(rotary_encoder)
        , button(button)
        , app_parser(app_parser)
{

}

//    if(pico_uart::singleton && pico_uart::singleton->obj && pico_uart::singleton->on_receive_callback)
//{
//    (pico_uart::singleton->obj->*pico_uart::singleton->on_receive_callback)(io_source::UART, &ch, 1);
//}
//
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

    blink_timer.create();

    return os::exit::OK;
}

uint8_t i = 0;
uint8_t i2 = 0;
int8_t rot = 0;

void app_display_handler::on_button_click(button::status status) OSAL_NOEXCEPT
{
    //NOTE: NO log in callback!!!!

    if(status == button::status::RELEASE)
    {
        i++;
    }


}

void app_display_handler::on_rotary_encoder_event(bool ccw, bool cw, bool click) OSAL_NOEXCEPT
{
    //NOTE: NO log in callback!!!!

    if(ccw)
    {
        rot--;
    }

    if(cw)
    {
        rot++;
    }

    if(click)
    {
        i2++;
    }

}

void app_display_handler::show_frame(bool wifi, const os::string<32> &now) const OSAL_NOEXCEPT
{
    os::lock_guard lg(mx);
    auto &&[display_width, display_height] = lcd->get_size();

    uint16_t ic_wifi_width = 0;
    uint16_t ic_wifi_height = 0;
    const uint8_t *ic_wifi = nullptr;
    uint8_t ic_wifi_size = 0;

    OSAL_LOG_DEBUG(APP_TAG, "i:%u, i2:%u rot:%d", i, i2, rot);

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

    if(now.length())
    {
        lcd->set_str(now.c_str(), display_width - (now.length() * 5) - 5, 1, font_5x8, sizeof(font_5x8));
    }

}

void app_display_handler::send_buffer() OSAL_NOEXCEPT
{
    os::lock_guard lg(mx);
    lcd->send_buffer();
}

void
app_display_handler::print_str(bool internal, const char str[], uint16_t y, enum valign valign, enum font font, int16_t offset_x) const OSAL_NOEXCEPT
{
    if(!internal && (app_parser.get_source() == iface::io_source::UART || app_parser.get_source() == iface::io_source::WIFI))
    {
        return;
    }
    if(str == nullptr || strlen(str) == 0)
    {
        return;
    }

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

void app_display_handler::clean(bool internal) const OSAL_NOEXCEPT
{
    if(!internal && (app_parser.get_source() == iface::io_source::UART || app_parser.get_source() == iface::io_source::WIFI))
    {
        return;
    }
    auto &&[display_width, display_height] = lcd->get_size();

    lcd->set_rect(0, 12, display_width, display_height - 12, write_mode::REMOVE);
}

auto app_display_handler::blink_timer_handler(os::timer *, void *) -> void *
{

    if(singleton->blink_show)
    {

        singleton->clean(true);

        string<64> msg = "Device locked by: ";
        msg += singleton->app_parser.get_user_logged();

        singleton->print_str(true, msg.c_str(), 26, valign::CENTER, font::F5X8);

        if(singleton->app_parser.is_user_logged())
        {
            switch(singleton->app_parser.get_source())
            {
                case iface::v1::io_source::UART:
                    singleton->print_str(true, "from UART", 45, valign::CENTER, font::F5X8);
                    break;
                case iface::v1::io_source::WIFI:
                    singleton->print_str(true, "from WIFI", 45, valign::CENTER, font::F5X8);
                    break;
                default:
                    break;
            }
        }
        singleton->blink_show = false;
    }
    else
    {
        singleton->clean(true);
        singleton->blink_show = true;
    }
    singleton->send_buffer();
    return nullptr;
}

void app_display_handler::lock()
{
    blink_timer.start();
}

inline void app_display_handler::on_logout() const
{
    blink_timer.stop();
}

}
}
