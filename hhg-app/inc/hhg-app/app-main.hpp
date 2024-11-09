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

#include "hhg-iface/initializable.hpp"
#include "hhg-iface/wifi.hpp"
#include "hhg-driver/hardware.hpp"
#include "osal/osal.hpp"
#include "hhg-app/app-config.hpp"
#include "hhg-app/app-data.hpp"
#include "hhg-app/app-parser.hpp"
#include "hhg-app/app-led.hpp"
#include "hhg-app/app-display-handler.hpp"
#include "hhg-app/app-mqtt.hpp"

namespace hhg::app
{
inline namespace v1
{


class app_main final : public hhg::iface::initializable
{
public:
    constexpr static const int16_t ONE_SEC_IN_MILLIS = 1'000;
    constexpr static const int32_t ONE_HOUR_IN_MILLIS = 60 * 60 * 1'000;
    static constexpr time_t TIMESTAMP_2020 = 1'577'880'000;

    enum state : uint32_t
    {
        CHECK_USERS                 = 0x00
        , CHECK_WIFI_ENABLED        = (1 << 0)
        , TRY_WIFI_CONNECTION       = (1 << 1)
        , CHECK_WIFI_WAIT_PARAMS    = (1 << 2)
        , CHECK_WIFI_WAIT_CONNECTION= (1 << 3)
        , CONNECTED                 = (1 << 4)
        , CHECK_TIMESTAMP           = (1 << 5)
        , SINCH_TIMESTAMP           = (1 << 6)
        , READY                     = (1 << 7)
        , EXECUTE_ZONE              = (1 << 8)

        , ERROR                     = (1 << 9)
    };

private:
    static inline app_main *singleton = nullptr;
    static constexpr uint64_t FSM_SLEEP = 100;

    const driver::hardware &hardware;
    hhg::app::app_config app_config;
    hhg::app::app_data app_data;
    hhg::app::app_parser app_parser;
    hhg::app::app_led app_led;
    hhg::app::app_display_handler app_display_handler;
    hhg::app::app_mqtt app_mqtt;

    static void *handler(void *arg);

    os::thread fsm_thread{"app_main", hhg::driver::HIGH, 1'024 * 2, handler};

    mutable struct fsm
    {
        static constexpr uint8_t MAX_ERROR = 5;

        enum state state = state::CHECK_USERS;
        enum state old_state = state::CHECK_USERS;
        uint8_t errors = 0;
        os::event events;
        bool run = true;
    } fsm;


public:
    explicit app_main(driver::hardware &hardware, os::error **error) OSAL_NOEXCEPT;
    OSAL_NO_COPY_NO_MOVE(app_main)

    ~app_main() override OSAL_NOEXCEPT;

    os::exit init(class os::error **error) OSAL_NOEXCEPT override;

    os::exit fsm_start(class os::error **error) OSAL_NOEXCEPT;

    uint32_t get_fsm_state() const OSAL_NOEXCEPT;

    static os::string<32> state_to_string(app_main::state state);
private:

    os::exit handle_error() OSAL_NOEXCEPT;

};

}
}

