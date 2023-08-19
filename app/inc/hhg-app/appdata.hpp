/***************************************************************************
 *
 * Hi Happy Garden Interfaces
 * Copyright (C) 2023  Antonio Salsi <passy.linux@zresa.it>
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
#include "hhg-intf/hardware.hpp"
#include "osal/osal.hpp"
#include "config.h"

namespace hhg::app
{
inline namespace v1
{

using os::string;
using os::unique_ptr;
using os::error;
namespace intf = hhg::intf;

enum class status
{
    UNACTIVE,
    ACTIVE,
    RUN
};

/**
 * @brief pod who describe a irrigation station
 */
struct zone final
{

    using ptr = unique_ptr<zone>;

    /**
    * @brief name of station
    */
    string<32> name;

    /**
    * @brief description of station
    */
    string<128> description;

    /**
    * @brief relay number association
    */
    uint8_t relay_number;

    /**
    * @brief watering time in minutes
    */
    uint watering_time;

    /**
    * @brief for manage order of execution lighter is first then weightier
    */
    uint16_t weight;

    /**
     * @brief status of station
     */
    enum status status;
};

struct schedule final
{

    using ptr = unique_ptr<schedule>;

    static inline const constexpr uint16_t NOT_SET = 0xFFFF;

    /**
    * @brief minute, values allowed 0 - 59
    */
    uint8_t minute = 0; //0 - 59 or NOT_SET

    /**
    * @brief minute, values allowed 0 - 23 or NOT_SET
    */
    uint8_t hour = 0; //0 - 23 or NOT_SET

    /**
    * @brief days, values allowed 0x01 - 0x7F or NOT_SET
    */
    union
    {
        struct
        {

            uint8_t mon: 1;
            uint8_t tue: 1;
            uint8_t wen: 1;
            uint8_t thu: 1;
            uint8_t fri: 1;
            uint8_t sat: 1;
            uint8_t sun: 1;
        };
        uint8_t data = static_cast<uint8_t>(NOT_SET);
    }days;

    /**
    * @brief months, values allowed 0x01 - 0xFFF or NOT_SET
    */
    union
    {
        struct
        {

            uint8_t jan: 1;
            uint8_t feb: 1;
            uint8_t mar: 1;
            uint8_t apr: 1;
            uint8_t may: 1;
            uint8_t jun: 1;
            uint8_t jul: 1;
            uint8_t aug: 1;
            uint8_t sep: 1;
            uint8_t oct: 1;
            uint8_t nov: 1;
            uint8_t dec: 1;
        };
        uint16_t data = NOT_SET;
    }months;

    /**
     * @brief brief description of aggregation
     */
    string<128> description;

    /**
     * @brief status of station
     */
    enum status status = status::ACTIVE;

    /**
     * @brief number of station
     */
    zone zones[HHGARDEN_ZONES_SIZE];
};

class app_data final
{
    const intf::hardware& hardware;

    schedule schedules[HHGARDEN_SCHEDULES_SIZE];

    bool init = false;
public:
    inline explicit app_data(const intf::hardware& hardware) OS_NOEXCEPT : hardware(hardware) {};
    OS_NO_COPY_NO_MOVE(app_data)

    bool load(error **error) OS_NOEXCEPT;

    bool save(error **error) const OS_NOEXCEPT;


};

}
}

