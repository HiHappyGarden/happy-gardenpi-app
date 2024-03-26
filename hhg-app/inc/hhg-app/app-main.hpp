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
using namespace os;


void* fsm_thread_handler(void* arg);

class app_main final : public hhg::iface::initializable
{
public:
    enum state
    {
    	NONE         	= 0x00,
        INIT         	= (1 << 0),
		CHECK_ZONE   	= (1 << 1),
		EXECUTE_ZONE 	= (1 << 2),
		SINCH_TIMESTAMP = (1 << 3),

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

	thread fsm_thread{"fsm thread", hhg::driver::HIGH, 1024 * 2, fsm_thread_handler};
    struct fsm
    {
		static constexpr uint8_t MAX_ERROR = 5;

        enum state   state       = state::NONE;
        enum state   old_state 	 = state::NONE;
        uint8_t      errors      = 0;
        event    	 events;
        bool         run         = true;
    }fsm;


	friend void* fsm_thread_handler(void* arg);
public:
    explicit app_main(driver::hardware& hardware, error** error) OS_NOEXCEPT;
    OS_NO_COPY_NO_MOVE(app_main)

    ~app_main() override OS_NOEXCEPT;

    os::exit init(class error** error) OS_NOEXCEPT override;

    os::exit fsm_start(class error** error) OS_NOEXCEPT;

private:

    os::exit handle_error() OS_NOEXCEPT;

};

}
}

