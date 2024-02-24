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

#include <hhg-app/app-main.hpp>
using hhg::iface::time;

#include "hhg-app/app-parser-commands.hpp"

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "APP MAIN";

string<32> state_to_string(app_main::state state)
{
	string<32> ret;
	switch (state)
	{
		case app_main::NONE:
			ret = "NONE";
			break;
		case app_main::INIT:
			ret = "INIT";
			break;
	}
	return ret;
}

}

void* fsm_thread_handler(void* arg)
{
	if(arg == nullptr)
	{
		app_main::singleton->fsm.run = false;
		OS_LOG_INFO(APP_TAG, "Arg nullptr");
	}

	error** error = static_cast<os::error**>(arg);
	time_t timer = 0;

	while(app_main::singleton->fsm.run)
	{
		time_t&& now = app_main::singleton->hardware.get_time()->get_timestamp(error);

		switch (app_main::singleton->fsm.state)
		{
			case app_main::NONE:
			{

				if(timer ==  0 && now > 0)
				{

					app_main::singleton->fsm.state = app_main::INIT;
					app_main::singleton->fsm.old_state = app_main::NONE;
					OS_LOG_INFO(APP_TAG, "state:%s - OK", state_to_string(app_main::singleton->fsm.state));
				}
				else
				{
					if(timer == 0)
					{
						tm date_time_default;
						app_main::singleton->hardware.get_time()->set_timestamp(mktime(&date_time_default), error);
						timer = 1_s;
						OS_LOG_WARNING(APP_TAG, "state:%s - timestamp not valid", state_to_string(app_main::singleton->fsm.state));
					}
					else
					{
						timer -= app_main::FSM_SLEEP;
					}
				}

				break;
			}
			case app_main::INIT:
			{
				break;
			}
			default:
			{
				break;
			}
		}
		us_sleep(app_main::FSM_SLEEP);
	}

	app_main::singleton->fsm_thread.exit();
	OS_LOG_DEBUG(APP_TAG, "Exit thread");

	return nullptr;
}


app_main::app_main(driver::hardware& hardware, class error** error) OS_NOEXCEPT
: hardware(hardware)
, app_config(hardware.get_fsio())
, app_data(hardware.get_fsio())
, app_parser(hardware.get_io())
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
}

app_main::~app_main() OS_NOEXCEPT = default;

os::exit app_main::init(class os::error** error) OS_NOEXCEPT
{
	OS_LOG_INFO(APP_TAG, "Init APP PARSER");
	hardware.get_io()->set_on_receive(&app_parser, &hhg::iface::io_on_receive::on_receive);

	if(app_parser.init(error) == exit::KO)
	{
		return exit::KO;
	}
	set_app_parser(app_parser);
	OS_LOG_INFO(APP_TAG, "Init APP PARSER - OK");

	OS_LOG_INFO(APP_TAG, "Init APP CONFIG");
	if(app_config.init(error) == exit::KO)
	{
		OS_LOG_WARNING(APP_TAG, "Load default config");
		if(app_config.load_defaut(error) == exit::KO)
		{
			if(error && *error)
			{
				*error = OS_ERROR_APPEND(*error, "Load default config fail", error_type::OS_ENOENT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
		OS_LOG_WARNING(APP_TAG, "Load default config - OK");
	}
	if(set_app_config(app_config, error) == exit::KO)
	{
		OS_LOG_WARNING(APP_TAG, "Load default config");
		if(app_config.load_defaut(error) == exit::KO)
		{
			if(error)
			{
				*error = OS_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
	}
	OS_LOG_INFO(APP_TAG, "Init APP CONFIG - OK");


	OS_LOG_INFO(APP_TAG, "Init APP DATA");
	if(app_data.init(error) == exit::KO)
	{
		if(error && *error)
		{
			printf_stack_error(APP_TAG, *error);
			delete (*error);
			*error = nullptr;
		}
		OS_LOG_WARNING(APP_TAG, "Impossible load data, reset");
		app_data.reset();

		OS_LOG_WARNING(APP_TAG, "Store default data");
		if(app_data.store(error) == exit::KO)
		{
			if(error)
			{
				*error = OS_ERROR_BUILD("set_app_config() fail.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
		OS_LOG_WARNING(APP_TAG, "Store default data - OK");
	}
	if(set_app_data(app_data, error) == exit::KO)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("set_app_data() fail.", error_type::OS_EFAULT);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init APP DATA - OK");

	OS_LOG_INFO(APP_TAG, "Set timer to parser");
	set_time(const_cast<class time*>(hardware.get_time().get()));


	return os::exit::OK;
}

os::exit app_main::fsm_start(class os::error** error) OS_NOEXCEPT
{
	return fsm_thread.create(error, error);
}


}
}

