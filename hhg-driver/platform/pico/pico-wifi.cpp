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

    os::exit pico_wifi::init(os::error **error)
    {

        if (cyw43_arch_init())
        {
            OS_LOG_ERROR(APP_TAG, "Failed to initialise");
            return exit::KO;
        }
        cyw43_arch_enable_sta_mode();

        if (cyw43_arch_wifi_connect_timeout_ms("Vodafone-salsi.local", "s4ls3tt4", CYW43_AUTH_WPA2_AES_PSK, 30'000))
        {
            OS_LOG_ERROR(APP_TAG, "Failed to connect");
            return exit::KO;
        }

        return exit::OK;
    }
}
}