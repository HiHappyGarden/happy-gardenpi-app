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

#ifdef INCLUDE_HHG_CONFIG
#include "hhg-config.h"
#endif

#include "hhg-iface/fs-io.hpp"
#include "hhg-iface/initializable.hpp"
#include "hhg-iface/file-version.hpp"

#include <time.h>
#include <stdint.h>

namespace hhg::app
{
inline namespace v1
{


enum class status : uint8_t
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

    using ptr = os::unique_ptr<zone>;

    /**
    * @brief description of zone
    */
    os::string<128> description;

    /**
    * @brief relay number association
    */
    uint8_t relay_number = 0;

    /**
    * @brief watering time in minutes less then 1 minute it's an error
    */
    uint8_t watering_time = 1;

    /**
    * @brief for manage order of execution lighter is first then weightier
    */
    uint16_t weight = 0;

    /**
     * @brief status of station
     */
    enum status status = status::UNACTIVE;
};

struct schedule final
{

    using ptr = os::unique_ptr<schedule>;

    static inline const constexpr uint16_t NOT_SET = 0xFFFF;

    /**
    * @brief minute, values allowed 0 - 59
    */
    uint8_t minute = static_cast<uint8_t>(NOT_SET); //0 - 59 or NOT_SET

    /**
    * @brief minute, values allowed 0 - 23 or NOT_SET
    */
    uint8_t hour = static_cast<uint8_t>(NOT_SET); //0 - 23 or NOT_SET

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
    os::string<128> description;

    /**
     * @brief status of station
     */
    enum status status = status::UNACTIVE;

    /**
     * @brief zones_len count number of zone configurated
     */
    uint8_t zones_len = 0;

    /**
     * @brief number of station
     */
    zone zones[HHG_ZONES_SIZE];
};


class app_data final : public hhg::iface::initializable
{
	constexpr static const uint32_t MAGIC = 0x2E'44'41'54;
	constexpr static const uint8_t VERSION = 1;

	const hhg::iface::fs_io::ptr& fsio;

	mutable struct alignas(2) data final : public hhg::iface::file_version
	{
		inline data() OSAL_NOEXCEPT : file_version{MAGIC, VERSION} {}
		schedule schedules[HHG_SCHEDULES_SIZE];
		uint32_t crc = MAGIC;
	} data;

public:

	using on_vesrion_change = void (*)(uint8_t version);

	explicit app_data(const hhg::iface::fs_io::ptr& fsio) OSAL_NOEXCEPT;
	~app_data() override;
	OSAL_NO_COPY_NO_MOVE(app_data)

	os::exit init(os::error** error) OSAL_NOEXCEPT override;

	void reset() OSAL_NOEXCEPT;

	os::exit store(os::error** error) const OSAL_NOEXCEPT;

	os::exit load(app_data::on_vesrion_change on_version_change, os::error** error) OSAL_NOEXCEPT;

	os::exit load_default(os::error** error) OSAL_NOEXCEPT;

    os::exit clear(os::error** error) const OSAL_NOEXCEPT;

	bool get_schedule(time_t timestamp, struct schedule& schedule) OSAL_NOEXCEPT;

	os::exit set_schedule(const char[]) OSAL_NOEXCEPT;

	os::exit set_zone(const char[]) OSAL_NOEXCEPT;

	char* get_schedule(uint8_t id) const OSAL_NOEXCEPT;

	char* get_zone(uint8_t id_schedule, uint8_t id) const OSAL_NOEXCEPT;

    const schedule* get_data(uint8_t id_schedule) const OSAL_NOEXCEPT;

    os::pair<uint8_t, const zone*> get_data(uint8_t id_schedule, uint8_t id) const  OSAL_NOEXCEPT;

private:
	static uint8_t get_bit_day(const tm* now) OSAL_NOEXCEPT;

};

}
}

