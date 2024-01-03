/*
 * appio.cpp
 *
 *  Created on: Jan 3, 2024
 *      Author: antoniosalsi
 */

#include <hhg-app/appparser.hpp>

namespace hhg::app
{
inline namespace v1
{

app_parser::app_parser(const hhg::iface::io::ptr& io) OS_NOEXCEPT
: io(io)
{

}


app_parser::~app_parser() OS_NOEXCEPT = default;


void app_parser::on_read(const uint8_t data[], uint16_t size) const OS_NOEXCEPT
{

	int i = 2;

}

}
}
