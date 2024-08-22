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

#include "hhg-iface/button.hpp"
#include "hhg-iface/event-exit.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_display_handler;
class app_parser;
class app_display_irrigate_now final : public hhg::iface::event_exit
{
    int16_t& menu_idx;
    class app_display_handler& app_display_handler;
    const hhg::app::app_parser& app_parser;
    hhg::iface::event_exit* obj = nullptr;
    hhg::iface::event_exit::on_exit_callback on_exit_callback = nullptr;

public:
    app_display_irrigate_now(class app_display_handler& app_display_handler, const hhg::app::app_parser& app_parser, int16_t& menu_idx, hhg::iface::event_exit* obj, hhg::iface::event_exit::on_exit_callback on_exit) OSAL_NOEXCEPT;
    ~app_display_irrigate_now() override = default;
    OSAL_NO_COPY_NO_MOVE(app_display_irrigate_now)

private:
    void on_exit(os::exit exit, const char* string, void *) override;
};

}
}