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

    os::exit pico_wifi::init(os::error **error)
    {
#if HHG_DISABLE_WIFI == 1
        if (cyw43_arch_init())
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_uart::init() cyw43_arch_init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }

        cyw43_arch_enable_sta_mode();
#endif

        return exit::OK;
    }

    os::exit pico_wifi::connect(const string<128> &user, const string<64> &passwd, enum auth auth, error **error) const OS_NOEXCEPT
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
#if HHG_DISABLE_WIFI == 1
        if (cyw43_arch_wifi_connect_timeout_ms(passwd.c_str(), passwd.c_str(), pico_auth, TIMEOUT))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_uart::connect() cyw43_arch_init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }

        OS_LOG_DEBUG(APP_TAG, "Connected");

#else
        OS_LOG_WARNING(APP_TAG, "WIFI disabled");
#endif
        return exit::OK;
    }




}
}