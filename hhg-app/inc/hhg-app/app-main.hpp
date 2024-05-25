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

#pragma once
#include "hhg-iface/initializable.hpp"
#include "hhg-driver/hardware.hpp"
#include "osal/osal.hpp"
#include "hhg-app/app-config.hpp"
#include "hhg-app/app-data.hpp"
#include "hhg-app/app-parser.hpp"

namespace hhg::app
{
inline namespace v1
{


void* fsm_thread_handler(void* arg);

//TODO: implement REAL singleton
class app_main final : public hhg::iface::initializable
{
public:
    enum state
    {
        NO_WIFI_CONF    = 0x00,
        NO_USERS        = (1 << 0),
    	NO_TIMESTAMP    = (1 << 1),
        INIT         	= (1 << 2),
		CHECK_ZONE   	= (1 << 3),
		EXECUTE_ZONE 	= (1 << 4),
		SINCH_TIMESTAMP = (1 << 5),

		ERROR  		 	= (1 << 7)

    };

private:
    static inline app_main* singleton = nullptr;
    static constexpr uint64_t FSM_SLEEP = 100;
	static constexpr time_t TIMESTAMP_2020 = 1'577'880'000;

    const driver::hardware& hardware;
    hhg::app::app_config app_config;
    hhg::app::app_data app_data;
    hhg::app::app_parser app_parser;

	os::thread fsm_thread{"fsm", hhg::driver::HIGH, 1024 * 2, fsm_thread_handler};
    struct fsm
    {
		static constexpr uint8_t MAX_ERROR = 5;

        enum state   state       = state::NO_WIFI_CONF;
        enum state   old_state 	 = state::NO_WIFI_CONF;
        uint8_t      errors      = 0;
        os::event    	 events;
        bool         run         = true;
    }fsm;


	friend void* fsm_thread_handler(void* arg);
public:
    explicit app_main(driver::hardware& hardware, os::error** error) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() override OS_NOEXCEPT;

    os::exit init(class os::error** error) OS_NOEXCEPT override;

    os::exit fsm_start(class os::error** error) OS_NOEXCEPT;

private:

    os::exit handle_error() OS_NOEXCEPT;

};

}
}

