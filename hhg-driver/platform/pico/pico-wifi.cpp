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

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif

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

    pico_wifi::~pico_wifi() OSAL_NOEXCEPT
    {
#if HHG_WIFI_DISABLE == 0
        cyw43_arch_deinit();
#endif
    }

    void *pico_wifi::handle(void *arg)
    {
        if(singleton == nullptr)
        {
            return nullptr;
        }

        if (cyw43_arch_init())
        {
            OSAL_LOG_FATAL(APP_TAG, "Failed to initialise");
            return nullptr;
        }
        cyw43_arch_enable_sta_mode();

        singleton->ntp.pcb = udp_new_ip_type(IPADDR_TYPE_V4);

        if (!singleton->ntp.pcb)
        {
            OSAL_LOG_FATAL(APP_TAG, "Failed to create pcb");
            return nullptr;
        }

        udp_recv(singleton->ntp.pcb, ntp_recv, &singleton->ntp);

        const uint8_t TICK = 100;

        while(singleton)
        {
            switch(singleton->fsm_state)
            {
                case fsm_state::NONE:
                    break;
                case fsm_state::DISCONNECTED:
                    osal_us_sleep(1'000_ms);

                    singleton->fsm_state = fsm_state::WAIT_CONNECTION;
                    break;
                case fsm_state::WAIT_CONNECTION:
                {
                    if (singleton->connection_timeout == 0)
                    {
                        singleton->fsm_state = fsm_state::DISCONNECTED;
                        singleton->events.clear(0xFFFF);
                        singleton->ip_addr = {0};
                    }
                    else if(singleton->connection_timeout)
                    {
                        singleton->connection_timeout -= TICK;
                    }

                    bool connected = netif_is_link_up(netif_default);
                    auto wifi_link_status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
                    if( connected && !(wifi_link_status & CYW43_LINK_DOWN) )
                    {
                        singleton->fsm_state = fsm_state::CONNECTED;
                    }

                    break;
                }
                case fsm_state::CONNECTED:
                {
                    bool connected = netif_is_link_up(netif_default);
                    auto wifi_link_status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
                    if(!connected || wifi_link_status & CYW43_LINK_DOWN)
                    {
                        singleton->fsm_state = fsm_state::DISCONNECTED;
                        singleton->events.clear(0xFFFF);
                        singleton->ip_addr = {0};
                        break;
                    }

                    uint32_t events = singleton->events.get();
                    if(events == fsm_state::DISCONNECTED)
                    {
                        singleton->events.set(fsm_state::CONNECTED);
                    }

                    if( !(events & fsm_state::HAS_IP) )
                    {
                        singleton->fsm_state = fsm_state::WAIT_IP;
                    }

                    break;
                }
                case fsm_state::WAIT_IP:
                {
                    auto ip_ready = dhcp_supplied_address(&cyw43_state.netif[CYW43_ITF_STA]);
                    if(ip_ready)
                    {
                        singleton->connection_timeout = 0;
                        singleton->ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr;
                        OSAL_LOG_DEBUG(APP_TAG, "Connected to ip %s", ip4addr_ntoa(&cyw43_state.netif[CYW43_ITF_STA].ip_addr));
                        singleton->fsm_state = fsm_state::HAS_IP;
                    }
                    break;
                }
                case fsm_state::HAS_IP:
                {
                    singleton->fsm_state = fsm_state::CONNECTED;
                    singleton->events.set(fsm_state::HAS_IP);
                    break;
                }
            }

            //OSAL_LOG_DEBUG(APP_TAG, "singleton->fsm_state:%u", singleton->fsm_state);
            osal_us_sleep(ms_to_us(TICK));
        }

        if(singleton)
        {
            singleton->thread.exit();
        }

        return nullptr;
    }

    os::exit pico_wifi::init(class error **error)
    {
        if(singleton)
        {
            if(error)
            {
                *error = OSAL_ERROR_BUILD("pico_wifi::init() fail.", error_type::OS_EFAULT);
                OSAL_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
        singleton = this;

#if HHG_WIFI_DISABLE == 0
        return thread.create();
#else
        return exit::OK;
#endif
    }

    os::exit pico_wifi::connect(const string<32> &ssid, const string<64> &passwd, enum auth auth, class error **error) OSAL_NOEXCEPT
    {
        connection_timeout = HHG_WIFI_CONNECTION_TIMEOUT;
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

        fsm_state = fsm_state::WAIT_CONNECTION;

#if HHG_WIFI_DISABLE == 0

        if (int32_t rc = cyw43_arch_wifi_connect_async(ssid.c_str(), passwd.c_str(), pico_auth); rc)
        {
            if(error)
            {
                *error = OSAL_ERROR_BUILD("pico_uart::connect() cyw43_arch_init() fail.", rc);
                OSAL_ERROR_PTR_SET_POSITION(*error);
            }
            return exit::KO;
        }
#elif HHG_WIFI_DISABLE == 1
        us_sleep(500_ms);
        OSAL_LOG_DEBUG(APP_TAG, "Connected to ip FAKE IP");
        singleton->events.set(fsm_state::CONNECTED | fsm_state::HAS_IP);
#elif HHG_WIFI_DISABLE == 2
#warning The connection wifi will fail
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
        if(p == nullptr)
        {
            singleton->ntp.state = ntp::state::NONE;
            return;
        }

        auto req = static_cast<uint8_t*>(p->payload);
        memset(req, 0, HHG_NTP_MSG_LEN);
        req[0] = 0x1b;
        udp_sendto(state->pcb, p, &state->server_address, HHG_NTP_PORT);

        singleton->ntp.state = ntp::state::REQUEST;

        pbuf_free(p);
        cyw43_arch_lwip_end();
    }


    void pico_wifi::ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg)
    {
        auto state = static_cast<struct ntp*>(arg);
        if (ipaddr)
        {
            state->server_address = *ipaddr;
            OSAL_LOG_DEBUG(APP_TAG, "NTP address %s", ipaddr_ntoa(ipaddr));

            singleton->ntp.state = ntp::state::DNS_FOUND;

            ntp_request(state);
        }
        else
        {
            OSAL_LOG_DEBUG(APP_TAG, "NTP dns request failed");
            if(state->error && *state->error)
            {
                *state->error = OSAL_ERROR_APPEND(*state->error, "NTP dns request failed", error_type::OS_EADDRNOTAVAIL);
                OSAL_ERROR_PTR_SET_POSITION(*state->error);
            }

            singleton->ntp.state = ntp::state::NONE;
        }
    }



    void pico_wifi::ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
    {
        auto state = static_cast<struct ntp*>(arg);
        uint8_t mode = pbuf_get_at(p, 0) & 0x7;
        uint8_t stratum = pbuf_get_at(p, 1);

        // Check the result
        if (ip_addr_cmp(addr, &state->server_address) && port == HHG_NTP_PORT && p->tot_len == HHG_NTP_MSG_LEN && mode == 0x4 && stratum != 0)
        {
            uint8_t seconds_buf[4] = {0};
            pbuf_copy_partial(p, seconds_buf, sizeof(seconds_buf), 40);
            uint32_t seconds_since_1900 = seconds_buf[0] << 24 | seconds_buf[1] << 16 | seconds_buf[2] << 8 | seconds_buf[3];
            uint32_t seconds_since_1970 = seconds_since_1900 - NTP_DELTA;
            time_t epoch = seconds_since_1970;
            if(state->on_callback)
            {
                state->on_callback(exit::OK, epoch);
            }
            singleton->ntp.state = ntp::state::NONE;
        }
        else
        {
            if(state->error)
            {
                *state->error = OSAL_ERROR_APPEND(*state->error, "Invalid ntp response", error_type::OS_ECONNABORTED);
                OSAL_ERROR_PTR_SET_POSITION(*state->error);
            }

            OSAL_LOG_DEBUG(APP_TAG, "NTP request - KO");
            if(state->on_callback)
            {
                state->on_callback(exit::KO, 0);
            }
        }
        pbuf_free(p);
    }




    os::exit pico_wifi::ntp_start(on_ntp_received on_ntp_callback, struct error **error) OSAL_NOEXCEPT
    {
        ntp.on_callback = on_ntp_callback;
        ntp.error = error;

#if HHG_WIFI_DISABLE == 0
        cyw43_arch_lwip_begin();

        err_t err = dns_gethostbyname(HHG_NTP_SERVER, &ntp.server_address, ntp_dns_found, &ntp);
        if(err && ntp.error)
        {
            *ntp.error = new osal::error("dns_gethostbyname() fail", err, get_file_name(__FILE__), "pico_wifi::ntp_start", __LINE__);
            OSAL_ERROR_PTR_SET_POSITION(*ntp.error);
        }

        if(err == 0)
        {
            ntp.state = ntp::state::START;
        }

        cyw43_arch_lwip_end();

        return err == 0 ? exit::OK : exit::KO;
#else
        us_sleep(500_ms);
        OSAL_LOG_DEBUG(APP_TAG, "Connected to ip FAKE IP");
        if(ntp.on_callback)
        {
            ntp.on_callback(exit::OK, HHG_WIFI_DISABLE_NTP_TIMESTAMP);
        }
        return exit::OK;
#endif
    }

    string<15> pico_wifi::get_ip_address_str() const OSAL_NOEXCEPT
    {

        string<15> ret;
#if HHG_WIFI_DISABLE == 0
        ret += ip4addr_ntoa(&ip_addr);
#else
        ret += "FAKE IP";
#endif
        return ret;
    }

    void pico_wifi::wifi_scan(time_t scan_time_in_millis) OSAL_NOEXCEPT
    {
        absolute_time_t scan_time = nil_time;
        bool scan_in_progress = false;
        while(true)
        {
            if(absolute_time_diff_us(get_absolute_time(), scan_time) < 0)
            {
                if(!scan_in_progress)
                {
                    cyw43_wifi_scan_options_t scan_options = {0};
                    int err = cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, [](void *env, const cyw43_ev_scan_result_t *result)
                    {
                        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
                               result->ssid, result->rssi, result->channel,
                               result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
                               result->auth_mode);
                        return 0;
                    });
                    if(err == 0)
                    {
                        printf("\nPerforming wifi scan\n");
                        scan_in_progress = true;
                    }
                    else
                    {
                        printf("Failed to start scan: %d\n", err);
                        scan_time = make_timeout_time_ms(scan_time_in_millis); // wait 10s and scan again
                    }
                }
                else if(!cyw43_wifi_scan_active(&cyw43_state))
                {
                    scan_time = make_timeout_time_ms(scan_time_in_millis); // wait 10s and scan again
                    scan_in_progress = false;
                }
            }
        }

    }


}
}