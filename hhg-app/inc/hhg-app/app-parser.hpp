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


class app_parser final : public hhg::iface::initializable, public hhg::iface::io::receive, public hhg::parser::parser::auth
{
public:
    struct auth
    {
        using callback = void (auth::*)() OSAL_NOEXCEPT;

        virtual ~auth() = default;
        virtual void on_logout() OSAL_NOEXCEPT = 0;
    };


private:

	static constexpr const uint16_t BUFFER_SIZE = 512;
	static constexpr const uint16_t RET_SIZE = 256;
    static constexpr const uint32_t AUTH_TIMEOUT = 5 * 60 * 1'000;
	static constexpr const char STARTER_CHAR[] = "$";
	static constexpr const uint8_t KO[] = "KO\r\n";
    static constexpr const uint8_t OK[] = "OK\r\n";
	static constexpr const char NEW_LINE[] = "\r\n";

	class os::error** error = nullptr;

    hhg::iface::io* io_ifaces[3];
    mutable hhg::iface::io_source source;

	hhg::parser::parser parser;


    static void* handler(void* arg) OSAL_NOEXCEPT;
	os::thread thread {
		"app_parser"
		, hhg::driver::NORMAL
		, 1'024
        , app_parser::handler
	};

    static auto auth_timer_handler(os::timer*, void*)-> void*;
    os::timer auth_timer{ os::ms_to_us(1'000), auth_timer_handler };
    auth* obj = nullptr;
    auth::callback on_logout = nullptr;

    bool run = false;

	mutable os::stream_buffer buffer {
		BUFFER_SIZE / 2
        , sizeof(char)
	};

	static inline app_parser* singleton = nullptr;


    app_config::user user_logged;
    hhg::iface::io_source source_user_logged;
    uint32_t user_logged_timeout = 0;
public:

	explicit app_parser(class os::error** error = nullptr) OSAL_NOEXCEPT;
	~app_parser() override OSAL_NOEXCEPT;
	OSAL_NO_COPY_NO_MOVE(app_parser)

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

	inline const hhg::parser::parser& get_parser() const OSAL_NOEXCEPT
	{
		return parser;
	}

	inline hhg::iface::io_source get_source() const OSAL_NOEXCEPT
	{
		return source;
	}

    inline bool is_user_logged() const OSAL_NOEXCEPT
    {
        return !user_logged.is_empty();
    }

    inline const os::string<32>& get_user_logged() const OSAL_NOEXCEPT
    {
        return user_logged.user;
    }

    inline const hhg::iface::io_source get_source_user_logged() const OSAL_NOEXCEPT
    {
        return source_user_logged;
    }

	void on_receive(hhg::iface::io_source, const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT override;

    inline void set_on_logout(auth* obj, auth::callback on_logout) OSAL_NOEXCEPT
    {
        this->obj = obj;
        this->on_logout = on_logout;
    }

    void register_io(hhg::iface::io_source, hhg::iface::io*) OSAL_NOEXCEPT;

private:
    friend os::exit auth(const hhg::parser::cmd_data &data, const hhg::parser::entry *entry, os::error **error) OSAL_NOEXCEPT;
    friend os::exit set_app_parser(class app_parser& app_parser, os::error** error) OSAL_NOEXCEPT;

    os::exit on_auth(const hhg::parser::cmd_data& data, const hhg::parser::entry* entry, os::error** error) OSAL_NOEXCEPT override;

    void set_user_logged(const app_config::user& user_logged) OSAL_NOEXCEPT;

    void clear_user_logged() OSAL_NOEXCEPT;

};

}
}
