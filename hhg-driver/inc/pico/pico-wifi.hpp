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


#pragma once

#include "hhg-iface/wifi.hpp"
#include "hhg-driver/os-config.hpp"
#include "FreeRTOSConfig.h"

#include <pico/cyw43_arch.h>

namespace hhg::driver
{
inline namespace v1
{


    class pico_wifi final : public hhg::iface::wifi
    {
        static inline pico_wifi* singleton = nullptr;

        constexpr static uint32_t NTP_DELTA  = 2'208'988'800;
        constexpr static uint32_t TIMEOUT = 30'000;

        static void* handle(void* arg);
        static inline os::thread thread{"wifi", hhg::driver::HIGH, 1024, pico_wifi::handle};

        on_connection_event* obj = nullptr;
        on_connection_event::callback callback = nullptr;

        bool connected = false;

        struct ntp
        {
            ip_addr_t ntp_server_address;
            bool dns_request_sent;
            udp_pcb *ntp_pcb;
            absolute_time_t ntp_test_time;
            alarm_id_t ntp_resend_alarm;
        } state{};
        on_ntp_received on_ntp_callback = nullptr;
        os::error **error = nullptr;
    public:

        pico_wifi();
        ~pico_wifi() override OS_NOEXCEPT;
        OS_NO_COPY_NO_MOVE(pico_wifi)

        void set_change_connection(on_connection_event* obj, on_connection_event::callback callback) OS_NOEXCEPT override
        {
            this->obj = obj;
            this->callback = callback;
        }

        os::exit init(os::error **error) OS_NOEXCEPT override;

        os::exit connect(const os::string<32>& ssid, const os::string<64>& passwd, enum auth auth, os::error **error) const OS_NOEXCEPT override;

        os::exit ntp_start(on_ntp_received, os::error **error) OS_NOEXCEPT override;

    private:
        static void ntp_request(struct ntp* state);
        static void ntp_dns_found(const char *hostname, const ip_addr_t *ipaddr, void *arg);
        static void ntp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
    };


}
}