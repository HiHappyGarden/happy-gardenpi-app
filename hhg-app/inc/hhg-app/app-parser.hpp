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

#include "hhg-iface/io.hpp"
#include "hhg-iface/initializable.hpp"
#include "hhg-parser/parser.hpp"
#include "hhg-driver/os-config.hpp"
#include "hhg-app/app-config.hpp"


namespace hhg::app
{
inline namespace v1
{

//TODO: implement REAL singleton
class app_parser final : public hhg::iface::initializable, public hhg::iface::io_on_receive, public hhg::parser::parser::auth
{
	//friend void* app_parser_thread_handler(void* arg) OS_NOEXCEPT;

	static constexpr const uint16_t BUFFER_SIZE = 512;
	static constexpr const uint16_t RET_SIZE = 256;
    static constexpr const uint32_t AUTH_TIMEOUT = 60 * 1'000;
	static constexpr const char STARTER_CHAR[] = "$";
	static constexpr const uint8_t KO[] = "KO\r\n";
    static constexpr const uint8_t OK[] = "OK\r\n";
	static constexpr const char NEW_LINE[] = "\r\n";

	class os::error** error = nullptr;


	const hhg::iface::io::ptr& io;
	hhg::parser::parser parser;

    static void* handler(void* arg) OS_NOEXCEPT;
	os::thread thread {
		"parser"
		, hhg::driver::NORMAL
		, 1'024 * 2
        , app_parser::handler
	};

    static auto auth_timer_handler(os::timer*, void*)-> void*;
    os::timer auth_timer{ os::ms_to_us(1'000), auth_timer_handler };

    bool run = false;

	mutable os::stream_buffer buffer {
		BUFFER_SIZE / 4
		, 1
	};

	static inline app_parser* singleton = nullptr;


    app_config::user user_logged;
    mutable hhg::iface::io_source source;
    uint32_t user_logged_timeout = 0;
public:

	explicit app_parser(const hhg::iface::io::ptr& io, class os::error** error = nullptr) OS_NOEXCEPT;
	~app_parser() override OS_NOEXCEPT;
	OS_NO_COPY_NO_MOVE(app_parser)

	os::exit init(os::error** error) OS_NOEXCEPT override;

	inline const hhg::parser::parser& get_parser() const OS_NOEXCEPT
	{
		return parser;
	}

	inline hhg::iface::io_source get_source() const OS_NOEXCEPT
	{
		return source;
	}

    inline bool is_user_logged() const OS_NOEXCEPT
    {
        return !user_logged.is_empty();
    }

    inline void set_source(hhg::iface::io_source source) OS_NOEXCEPT
    {
        this->source = source;
    }

    inline hhg::iface::io_source get_source(hhg::iface::io_source source) const OS_NOEXCEPT
    {
        return source;
    }

	void on_receive(hhg::iface::io_source, const uint8_t data[], uint16_t size) const OS_NOEXCEPT override;

private:
    friend os::exit auth(const hhg::parser::cmd_data &data, const hhg::parser::entry *entry, os::error **error) OS_NOEXCEPT;
    friend os::exit set_app_parser(class app_parser& app_parser, os::error** error) OS_NOEXCEPT;

    os::exit on_auth(const hhg::parser::cmd_data& data, const hhg::parser::entry* entry, os::error** error) OS_NOEXCEPT override;

    void set_user_logged(const app_config::user& user_logged) OS_NOEXCEPT;

    void clear_user_logged() OS_NOEXCEPT;

};

}
}
