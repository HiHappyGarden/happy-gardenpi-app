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
#include "hhg-iface/rgb_led.hpp"
#include "hhg-driver/os-config.hpp"

namespace hhg::app
{
inline namespace v1
{

class app_led final  : public hhg::iface::initializable
{

    static inline app_led* singleton = nullptr;
    static constexpr uint8_t TICK = 100;


    const hhg::iface::rgb_led::ptr& rgb_led;

    static void* handler(void* arg) OSAL_NOEXCEPT;
    os::thread thread {
            "app_led"
            , hhg::driver::LOW
            , hhg::driver::MINIMAL_STACK_SIZE
            , handler
    };


    mutable enum class status : uint8_t
    {
        NONE,
        LOADING,
        WARNING,
        ERROR,
        READY,
        RUNNING_IRRIGATION
    } status = status::LOADING, curren_status = status::LOADING;

public:
    explicit app_led(const hhg::iface::rgb_led::ptr& rgb_led) OSAL_NOEXCEPT;
    ~app_led() override;
    OSAL_NO_COPY_NO_MOVE(app_led)

    os::exit init(os::error **error) OSAL_NOEXCEPT override;

    void loading() const OSAL_NOEXCEPT;

    void warning() const OSAL_NOEXCEPT;

    void error() const OSAL_NOEXCEPT;

    void ready() const OSAL_NOEXCEPT;

    void running_irrigation()  const OSAL_NOEXCEPT;

private:
};

}
}

