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

#include <hhg-app/app-parser.hpp>
using namespace os;

namespace hhg::app
{
inline namespace v1
{

app_parser::app_parser(const hhg::iface::io::ptr& io, class os::error** error) OS_NOEXCEPT
: error(nullptr)
, io(io)
{
	if(singleton)
	{
		if(error)
		{
	        *error = OS_ERROR_BUILD("Only one instance at a time", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return;
	}

	singleton = this;

	if(!run)
	{
		run = thread.create(nullptr, this->error) == exit::OK;
	}
	else
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("Thread already run", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return;
	}
}


app_parser::~app_parser() OS_NOEXCEPT
{
	singleton = nullptr;
	run = false;
}


void app_parser::on_receive(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{
	if(data)
	{
		buffer.send_from_isr(data, size, 10_ms, error);
	}
}

void* app_parser_thread_handler(void* arg) OS_NOEXCEPT
{
	string<app_parser::singleton->BUFFER_SIZE> buffer;

	while(app_parser::singleton->run)
	{
		char ch = '\0';
		if(app_parser::singleton->buffer.receive(reinterpret_cast<uint8_t *>(&ch), 1, WAIT_FOREVER))
		{
			buffer += ch;

			auto start = buffer.find(app_parser::STARTER_CHAR);
			auto end = buffer.find("\r\n");
			if (end)
			{
				end  = buffer.find("\n");
			}

			if (start && end)
			{



			}
		}
	}

	app_parser::singleton->thread.exit();

	return nullptr;
}

}
}
