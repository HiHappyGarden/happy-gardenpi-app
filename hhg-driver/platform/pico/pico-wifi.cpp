/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
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

#include "pico/pico-wifi.hpp"
using namespace os;

#include <pico/cyw43_arch.h>


namespace hhg::driver
{
inline namespace v1
{

    namespace
    {
        constexpr char APP_TAG[] = "DRV WIFI";
    }

    pico_wifi::pico_wifi() = default;

    pico_wifi::~pico_wifi() OS_NOEXCEPT
    {
        cyw43_arch_deinit();
    }

    void *pico_wifi::handle(void *arg)
    {
        if (cyw43_arch_init()) {
            OS_LOG_FATAL(APP_TAG, "Failed to initialise");
            return nullptr;
        }
        cyw43_arch_enable_sta_mode();

        while(singleton)
        {
            osal_us_sleep(100);

            bool connected = netif_is_link_up(netif_default);
            if(!singleton->connected && connected)
            {

                OS_LOG_DEBUG(APP_TAG, "Connected");
                if(singleton->obj && singleton->callback)
                {
                    (singleton->obj->*singleton->callback)(singleton->connected, connected);
                }
                singleton->connected = true;
            }
            else if(singleton->connected && !connected)
            {

                OS_LOG_DEBUG(APP_TAG, "Disconnected");
                if(singleton->obj && singleton->callback)
                {
                    (singleton->obj->*singleton->callback)(singleton->connected, connected);
                }
                singleton->connected = false;
            }
        }

        thread.exit();

        return nullptr;
    }

    os::exit pico_wifi::init(error **error)
    {
        if(singleton)
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_wifi::init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
        singleton = this;

        thread.create();

        return exit::OK;
    }

    os::exit pico_wifi::connect(const string<32> &ssid, const string<64> &passwd, enum auth auth, error **error) const OS_NOEXCEPT
    {
        uint32_t pico_auth = 0;
        switch (auth)
        {
            case auth::OPEN:
                pico_auth = CYW43_AUTH_OPEN;
                break;
            case auth::WPA_TKIP_PSK:
                pico_auth = CYW43_AUTH_WPA_TKIP_PSK;
                break;
            case auth::WPA2_AES_PSK:
                pico_auth = CYW43_AUTH_WPA2_AES_PSK;
                break;
            case auth::WPA2_MIXED_PSK:
                pico_auth = CYW43_AUTH_WPA2_MIXED_PSK;
                break;
        }

        if (cyw43_arch_wifi_connect_async(ssid.c_str(), passwd.c_str(), pico_auth))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_uart::connect() cyw43_arch_init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }

        return exit::OK;
    }


}
}