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

namespace hhg::app
{
inline namespace v1
{

void* app_parser_thread_handler(void* arg) OS_NOEXCEPT;

class app_parser final : public hhg::iface::io_on_receive
{
	friend void* app_parser_thread_handler(void* arg) OS_NOEXCEPT;

	static constexpr const uint16_t BUFFER_SIZE = 512;
	static constexpr const char STARTER_CHAR[] = "#";


	class os::error** error = nullptr;


	const hhg::iface::io::ptr& io;

	os::thread thread {
		"app_parser thread"
		, 4
		, 1024
		, app_parser_thread_handler
	};

	bool run = false;

	mutable os::stream_buffer buffer {
		BUFFER_SIZE / 4
		, 1
	};

	static inline app_parser* singleton = nullptr;

public:

	explicit app_parser(const hhg::iface::io::ptr& io, class os::error** error = nullptr) OS_NOEXCEPT;
	~app_parser() OS_NOEXCEPT;
	OS_NO_COPY_NO_MOVE(app_parser)

	void on_receive(const uint8_t data[], uint16_t size) const OS_NOEXCEPT override;


};

}
}
