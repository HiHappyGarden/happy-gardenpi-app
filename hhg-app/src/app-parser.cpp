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

#include "hhg-app/app-parser.hpp"
using hhg::iface::io_source;
using hhg::iface::io;

#include "hhg-app/app-parser-commands.hpp"
using namespace os;

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP-PARSER";

}

app_parser::app_parser(class error** error) OSAL_NOEXCEPT
: error(nullptr)
, parser(get_commands(), get_commands_size())
{
	if(singleton)
	{
		if(error)
		{
	        *error = OSAL_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return;
	}

    memset(io_ifaces, 0, sizeof(io_ifaces));
    parser.set_on_auth(this, &hhg::parser::parser::auth::on_auth);

	singleton = this;
}


app_parser::~app_parser() OSAL_NOEXCEPT
{
	singleton = nullptr;
	run = false;
}

os::exit app_parser::init(class error** error) OSAL_NOEXCEPT
{
	if(!run)
	{
		run = thread.create(nullptr, this->error) == exit::OK && auth_timer.create() == exit::OK;
	}
	else
	{
		if(error)
		{
			*error = OSAL_ERROR_BUILD("Thread already run", error_type::OS_EFAULT);
	        OSAL_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	return run ? exit::OK : exit::KO;
}

os::exit app_parser::send_cmd(hhg::iface::io_source source, const uint8_t data[], uint16_t size, string<app_parser::RET_SIZE>& ret) const OSAL_NOEXCEPT
{
    if(data == nullptr)
    {
        return exit::KO;
    }

    on_receive(source, data, size);

    uint8_t ret_buff[RET_SIZE] = { 0 };
    auto ret_buff_len = this->ret.receive(ret_buff, sizeof(ret_buff), 1_s, nullptr);
    if(ret_buff_len)
    {
        for(uint8_t i = 0; i < ret_buff_len; i++)
        {
            ret += static_cast<char>(ret_buff[i]);
        }

        return ret.find("OK") ? exit::OK : exit::KO;
    }

    return exit::KO;
}

void app_parser::register_io(io_source io_source, class io* io)
{
    auto idx = static_cast<uint8_t>(io_source);
    if(idx > static_cast<uint8_t>(io_source::DISPLAY))
    {
        return;
    }
    io_ifaces[static_cast<uint8_t>(io_source)] = io;
}

void app_parser::on_receive(io_source source, const uint8_t data[], uint16_t size) const OSAL_NOEXCEPT
{
    if(data)
    {
        this->source = source;
        switch(source)
        {
            case io_source::DISPLAY:
            case io_source::UART:
                buffer.send_from_isr(data, size, 10_ms, error);
                break;
            case io_source::WIFI:
                buffer.send(data, size, 10_ms, error);
                break;
        }

    }
}


os::exit app_parser::on_auth(const cmd_data &data, const entry *entry, class error **error) OSAL_NOEXCEPT
{
    if(strlen(entry->access) == 0)
    {
        return exit::OK;
    }
    if(data.tokens_len < 1)
    {
        return exit::KO;
    }

    char access[ACCESS_MAX];
    strncpy(access, entry->access, ACCESS_MAX);

    char* ptr = strstr(access, HHG_DIVISOR);
    if(ptr == nullptr)
    {
        if(user_logged.user == access)
        {
            return exit::OK;
        }

        return exit::KO;
    }

    *ptr = '\0';
    const char* first = access;
    const char* second = ptr + 1;

    if(user_logged.user == first)
    {
        return exit::OK;
    }
    else if(user_logged.user == second)
    {
        return exit::OK;
    }

    return exit::KO;
}

void app_parser::set_user_logged(const app_config::user& user_logged) OSAL_NOEXCEPT
{
    this->user_logged = user_logged;
    source_user_logged = source;
    auth_timer.start();
}

void app_parser::clear_user_logged() OSAL_NOEXCEPT
{
    this->user_logged = {};
    auth_timer.stop();
    if(obj && on_logout)
    {
        (obj->*on_logout)();
    }
}

void* app_parser::handler(void* arg) OSAL_NOEXCEPT
{
	string<app_parser::BUFFER_SIZE> buffer;
	string<app_parser::RET_SIZE> ret;

	while(app_parser::singleton->run)
	{
		char ch = '\0';
		if(app_parser::singleton->buffer.receive(reinterpret_cast<uint8_t *>(&ch), 1, WAIT_FOREVER))
		{
            class io* io = singleton->io_ifaces[static_cast<uint8_t>(singleton->source)];
			buffer += ch;

			auto start = buffer.find(app_parser::STARTER_CHAR);
			auto end = buffer.find("\r\n");
            if(end == nullptr)
            {
                end = buffer.find("\n");
            }
            if (start && end)
			{
                singleton->buffer.reset();
				class error* error = nullptr;
				if(singleton->parser.execute(buffer.substr(start - buffer.c_str(), end - start).c_str(), ret.c_str(), ret.size(), &error) == exit::OK)
				{
                    if(singleton->is_user_logged() && singleton->auth_timer.is_active())
                    {
                        singleton->user_logged_timeout = AUTH_TIMEOUT;
                    }

					ret += app_parser::NEW_LINE;
                    if(singleton->source == io_source::UART)
                    {
                        io->transmit(reinterpret_cast<const uint8_t*>(ret.c_str()), ret.length());;
                    }
                    else if(singleton->source == io_source::WIFI)
                    {
                        io->transmit(reinterpret_cast<const uint8_t*>(ret.rtrim().c_str()), ret.length());
                    }
                    else
                    {
                        singleton->ret.send(reinterpret_cast<const uint8_t*>(ret.rtrim().c_str()), ret.length(), 100_ms, nullptr);
                    }
				}
				else
				{
                    if(singleton->source == io_source::DISPLAY)
                    {
                        singleton->ret.send(reinterpret_cast<const uint8_t*>(app_parser::KO), sizeof(app_parser::KO) - 1, 100_ms, nullptr);
                    }
                    else
                    {
                        io->transmit(app_parser::KO, sizeof(app_parser::KO) - 1);
                        if(error)
                        {
                            printf_stack_error(APP_TAG, error);
                            delete error;
                            error = nullptr;
                        }
                    }

				}

				buffer.clear();
				ret.clear();
			}
            else if (start == nullptr && end)
			{
                singleton->buffer.reset();
				io->transmit(app_parser::KO, sizeof(app_parser::KO) - 1);
				buffer.clear();
				ret.clear();
			}
		}
	}

	singleton->thread.exit();

	return nullptr;
}

auto app_parser::auth_timer_handler(os::timer *, void *) -> void *
{
    if(singleton && singleton->user_logged_timeout == 0)
    {
        OSAL_LOG_DEBUG(APP_TAG, "Session timeout user:%s", singleton->user_logged.user.c_str());
        singleton->clear_user_logged();
        if(singleton->obj && singleton->on_logout)
        {
            (singleton->obj->*singleton->on_logout)();
        }
    }
    else if(singleton)
    {
        singleton->user_logged_timeout -= 1'000;
    }

    return nullptr;
}


}
}
