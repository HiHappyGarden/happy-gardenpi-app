/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 202X  Antonio Salsi <passy.linux@zresa.it>
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

#include "osal/osal.hpp"
#include "hhg-app/appdata.hpp"
#include "hhg-intf/data.hpp"

#include "config.h"



namespace hhg::app
{
inline namespace v1
{

using os::error;
using os::string;
namespace intf = hhg::intf;

bool parse(const string<intf::data::FILE_SIZE>& json, class conf& conf, class error** error);

string<intf::data::FILE_SIZE> print(const class conf& conf, class error** error);

bool parse(const string<intf::data::FILE_SIZE>& json, schedule (&schedules)[HHGARDEN_SCHEDULES_SIZE], class error** error);

string<intf::data::FILE_SIZE> print(const schedule (&schedules)[HHGARDEN_SCHEDULES_SIZE], class error** error);

}
}

