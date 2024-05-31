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
#include "hhg-config.h"
using namespace os;


#include <pico/time.h>
#include <lwip/dns.h>
#include <lwip/pbuf.h>
#include <lwip/udp.h>


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
#if HHG_NO_WIFI == 0
        cyw43_arch_deinit();
#endif
    }

    void *pico_wifi::handle(void *arg)
    {
        if (cyw43_arch_init())
        {
            OS_LOG_FATAL(APP_TAG, "Failed to initialise");
            return nullptr;
        }
        cyw43_arch_enable_sta_mode();

        singleton->state.ntp_pcb = udp_new_ip_type(IPADDR_TYPE_V4);

        if (!singleton->state.ntp_pcb)
        {
            OS_LOG_FATAL(APP_TAG, "Failed to create pcb");
            return nullptr;
        }

        udp_recv(singleton->state.ntp_pcb, ntp_recv, &singleton->state);

//        cyw43_arch_wifi_connect_timeout_ms("Vodafone-salsi.local", "s4ls3tt4", CYW43_AUTH_WPA2_AES_PSK, 10'000);


        while(singleton)
        {
            osal_us_sleep(100_ms);

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

    os::exit pico_wifi::init(class error **error)
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

#if HHG_NO_WIFI == 0
        thread.create();
#endif
        return exit::OK;
    }

    os::exit pico_wifi::connect(const string<32> &ssid, const string<64> &passwd, enum auth auth, class error **error) const OS_NOEXCEPT
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

#if HHG_NO_WIFI == 0
        if (cyw43_arch_wifi_connect_async(ssid.c_str(), passwd.c_str(), pico_auth))
        {
            if(error)
            {
                *error = OS_ERROR_BUILD("pico_uart::connect() cyw43_arch_init() fail.", error_type::OS_EFAULT);
                OS_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
#endif

        return exit::OK;
    }

    void pico_wifi::ntp_request(struct ntp* state)
    {
        // cyw43_arch_lwip_begin/end should be used around calls into lwIP to ensure correct locking.
        // You can omit them if you are in a callback from lwIP. Note that when using pico_cyw_arch_poll
        // these calls are a no-op and can be omitted, but it is a good practice to use them in
        // case you switch the cyw43_arch type later.
        cyw43_arch_lwip_begin();
        struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, HHG_NTP_MSG_LEN, PBUF_RAM);
        auto req = static_cast<uint8_t*>(p->payload);
        memset(req, 0, HHG_NTP_MSG_LEN);
        req[0] = 0x1b;
        udp_sendto(state->ntp_pcb, p, &state->ntp_server_address, HHG_NTP_PORT);
        pbuf_free(p);
        cyw43_arch_lwip_end();
    }


    void pico_wifi::ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg)
    {
        auto state = static_cast<struct ntp*>(arg);
        if (ipaddr)
        {
            state->ntp_server_address = *ipaddr;
            OS_LOG_DEBUG(APP_TAG, "NTP address %s", ipaddr_ntoa(ipaddr));
            ntp_request(state);
        }
        else
        {
            OS_LOG_DEBUG(APP_TAG, "NTP dns request failed");
            if(singleton && singleton->error && *singleton->error)
            {
                *singleton->error = OS_ERROR_APPEND(*singleton->error, "NTP dns request failed", error_type::OS_EADDRNOTAVAIL);
                OS_ERROR_PTR_SET_POSITION(*singleton->error);
            }
        }
    }



    void pico_wifi::ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
    {
        auto state = static_cast<struct ntp*>(arg);
        uint8_t mode = pbuf_get_at(p, 0) & 0x7;
        uint8_t stratum = pbuf_get_at(p, 1);

        // Check the result
        if (ip_addr_cmp(addr, &state->ntp_server_address) && port == HHG_NTP_PORT && p->tot_len == HHG_NTP_MSG_LEN && mode == 0x4 && stratum != 0)
        {
            uint8_t seconds_buf[4] = {0};
            pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
            uint32_t seconds_since_1900 = seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
            uint32_t seconds_since_1970 = seconds_since_1900 - NTP_DELTA;
            time_t epoch = seconds_since_1970;
            OS_LOG_DEBUG(APP_TAG, "NTP request - OK timestamp:%u", epoch);
            if(singleton && singleton->on_ntp_callback)
            {
                singleton->on_ntp_callback(exit::OK, epoch);
            }
        }
        else
        {
            if(singleton->pico_wifi::error)
            {
                *singleton->pico_wifi::error = OS_ERROR_APPEND(*singleton->pico_wifi::error, "Invalid ntp response", error_type::OS_ECONNABORTED);
                OS_ERROR_PTR_SET_POSITION(*singleton->pico_wifi::error);
            }

            OS_LOG_DEBUG(APP_TAG, "NTP request - KO");
            if(singleton && singleton->on_ntp_callback)
            {
                singleton->on_ntp_callback(exit::KO, 0);
            }
        }
        pbuf_free(p);
    }




    os::exit pico_wifi::ntp_start(on_ntp_received on_ntp_callback, struct error **error) OS_NOEXCEPT
    {
        pico_wifi::on_ntp_callback = on_ntp_callback;
        pico_wifi::error = error;

        cyw43_arch_lwip_begin();

        err_t err = dns_gethostbyname(HHG_NTP_SERVER, &state.ntp_server_address, ntp_dns_found, &state);
        if(err && pico_wifi::error)
        {
            *pico_wifi::error = new osal::error("dns_gethostbyname() fail", err, get_file_name(__FILE__), "pico_wifi::ntp_start", __LINE__);
            OS_ERROR_PTR_SET_POSITION(*pico_wifi::error);
        }

        cyw43_arch_lwip_end();

        return err == 0 ? exit::OK : exit::KO;
    }

}
}