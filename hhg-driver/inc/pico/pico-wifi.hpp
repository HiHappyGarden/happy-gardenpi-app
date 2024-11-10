/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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


#pragma once

#include "hhg-iface/wifi.hpp"
#include "hhg-iface/mqtt.hpp"
#include "hhg-driver/os-config.hpp"
#include "FreeRTOSConfig.h"

#include <pico/cyw43_arch.h>

namespace hhg::driver
{
inline namespace v1
{

    //TODO: add ssid and passwd when store data check connection and if are changed reconnect
    class pico_wifi final : public hhg::iface::wifi
    {
        static inline pico_wifi* singleton = nullptr;

        constexpr static uint32_t NTP_DELTA  = 2'208'988'800;
        constexpr static uint32_t TIMEOUT = 30'000;

        hhg::iface::mqtt::ptr& mqtt;

        static void* handle(void* arg);
        os::thread thread{"pico_wifi", HIGH, 1'024 * 3, handle};

        mutable os::event events;
        mutable int16_t connection_timeout = 0;

        struct ntp
        {
            ip_addr_t server_address { .addr = 0 };
            udp_pcb* pcb = nullptr;
            on_ntp_received on_callback = nullptr;
            os::error **error = nullptr;

            enum class state
            {
                NONE,
                START,
                DNS_FOUND,
                REQUEST
            } state = state::NONE;

        } ntp{};

        enum fsm_state fsm_state = fsm_state::NONE;
        ip_addr_t ip_addr { .addr = 0 };


    public:

        explicit pico_wifi(hhg::iface::mqtt::ptr& mqtt) OSAL_NOEXCEPT;
        ~pico_wifi() override OSAL_NOEXCEPT;
        OSAL_NO_COPY_NO_MOVE(pico_wifi)

        os::exit init(os::error **error) OSAL_NOEXCEPT override;

        os::exit connect(const os::string<32>& ssid, const os::string<64>& passwd, enum auth auth, os::error **error) OSAL_NOEXCEPT override;

        os::exit ntp_start(on_ntp_received, os::error **error) OSAL_NOEXCEPT override;

        os::string<15> get_ip_address_str() const OSAL_NOEXCEPT override;

        inline uint32_t get_ip_address()  const OSAL_NOEXCEPT override
        {
            return ip_addr.addr;
        }

        inline os::pair<bool, bool> is_connected() OSAL_NOEXCEPT override
        {
            auto ev = events.get();
            return {(ev & CONNECTED) == CONNECTED, (ev & HAS_IP) == HAS_IP};
        }

        inline uint32_t get_timeout() const OSAL_NOEXCEPT override
        {
            return TIMEOUT;
        }

        inline uint32_t get_fsm_state() const OSAL_NOEXCEPT override
        {
            return events.get();
        }

        inline bool is_ntp_synchronized()  const OSAL_NOEXCEPT override
        {
            return ntp.server_address.addr != 0;
        }

    private:
        static void ntp_request(struct ntp* state);
        static void ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg);
        static void ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
    };


}
}