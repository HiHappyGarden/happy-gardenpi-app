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


#include "stm32g4xx/stm32-fsio.hpp"
#include "stm32g4xx/stm32-freertos.hpp"
using namespace hhg::iface;
using namespace os;




#define DATA_32                     ((uint32_t)0x12345678)
#define DATA_64                     ((uint64_t)0x5f5f6369616f5f5f)


namespace hhg::driver
{
namespace v1
{

uint64_t const stm32_fsio::check_data = 0x0102030404030201;

stm32_fsio::stm32_fsio(uint32_t start_flash_address, uint32_t end_flash_address) OS_NOEXCEPT
: start_flash_address(start_flash_address)
, end_flash_address(end_flash_address)
{

}

stm32_fsio::~stm32_fsio()
{
	/* Lock the Flash to disable the flash control register access (recommended
	  to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();
}

os::exit stm32_fsio::init(error** error) OS_NOEXCEPT
{
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	/* Unlock the Options Bytes *************************************************/
	HAL_FLASH_OB_Unlock();

	options_bytes_struct1.WRPArea  = OB_WRPAREA_BANK1_AREAA;
	options_bytes_struct2.WRPArea = OB_WRPAREA_BANK1_AREAB;

	/* Get pages write protection status ****************************************/
	HAL_FLASHEx_OBGetConfig(&options_bytes_struct1);
	HAL_FLASHEx_OBGetConfig(&options_bytes_struct2);

	/* Configure write protected pages */
	if (options_bytes_struct1.OptionType == OPTIONBYTE_WRP)
	{
		if(HAL_FLASHEx_OBProgram(&options_bytes_struct1) != HAL_OK)
		{
			/* Error occurred while options bytes programming. **********************/
			if(error)
			{
				*error = OS_ERROR_BUILD("Error occurred while options bytes programming.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
	}

	if (options_bytes_struct2.OptionType == OPTIONBYTE_WRP)
	{
		if(HAL_FLASHEx_OBProgram(&options_bytes_struct2) != HAL_OK)
		{
			/* Error occurred while options bytes programming. **********************/
			if(error)
			{
				*error = OS_ERROR_BUILD("Error occurred while options bytes programming.", error_type::OS_EFAULT);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
	}

	/* Generate System Reset to load the new option byte values ***************/
	if ((options_bytes_struct1.OptionType == OPTIONBYTE_WRP) || (options_bytes_struct2.OptionType == OPTIONBYTE_WRP))
	{
		HAL_FLASH_OB_Launch();
	}

	/* Lock the Options Bytes *************************************************/
	HAL_FLASH_OB_Lock();


	return exit::OK;
}

os::exit stm32_fsio::write(data_type type, const uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{
	auto&& [start_address, end_address] = get_start_and_end_addresses(type, size, error);

	if(start_address.address == 0 && end_address.address == 0)
	{
		return exit::KO;
	}

	uint32_t start_page = get_page(start_address.address);
	uint32_t end_page   = get_page(end_address.address);

	FLASH_EraseInitTypeDef erase_init_struct;

	uint32_t bytes_writed = 0;
	uint32_t page_error = 0;

	/* The desired pages are not write protected */
	/* Fill EraseInit structure************************************************/
	erase_init_struct.TypeErase   = FLASH_TYPEERASE_PAGES;
	erase_init_struct.Banks       = FLASH_BANK_1;
	erase_init_struct.Page        = start_page;
	erase_init_struct.NbPages     = end_page - start_page + 1;


	if (HAL_FLASHEx_Erase(&erase_init_struct, &page_error) != HAL_OK)
	{
		/*
		Error occurred while page erase.
		User can add here some code to deal with this error.
		PageError will contain the faulty page and then to know the code error on this page,
		user can call function 'HAL_FLASH_GetError()'
		*/
		if(error)
		{
			*error = OS_ERROR_BUILD("Error occurred while page erase.", error_type::OS_EIO);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}


	while(bytes_writed < size)
	{
		uint64_t byte_to_write = 0;
		uint8_t i = 0;
		for(; i < 8 && (bytes_writed + i) < size; i++) //bigendiam +-
		{
			uint8_t byte = (data[i + bytes_writed]);
			byte_to_write |= (static_cast<uint64_t>(byte) << (i * 8));
		}
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, start_address.address + bytes_writed, byte_to_write) == HAL_OK)
		{
			bytes_writed += i;
		}
		else
		{
			/* Error occurred while writing data in Flash memory.
			User can add here some code to deal with this error */
			if(error)
			{
				*error = OS_ERROR_BUILD("Error occurred while writing data in Flash memory.", error_type::OS_EIO);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
	}

	return exit::OK;
}

os::exit stm32_fsio::read(data_type type, uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{
    /* Check the correctness of written data */
	auto&& [start_address, end_address] = get_start_and_end_addresses(type, size, error);

	if(start_address.address == 0 && end_address.address == 0)
	{
		return exit::KO;
	}

	if( (start_address.address + size) > end_address.address)
	{
		size = end_address.address - start_address.address;
	}
	memcpy(data, reinterpret_cast<const uint32_t*>(start_address.address), size);

	return exit::OK;
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t stm32_fsio::get_page(uint32_t address) OS_NOEXCEPT
{
  uint32_t page = 0;

  if (address < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (address - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

pair<stm32_fsio::meta_address, stm32_fsio::meta_address> stm32_fsio::get_start_and_end_addresses(iface::v1::data_type type, size_t size, error** error) const OS_NOEXCEPT
{
	uint32_t start_address = 0;
	uint32_t end_address = 0;

	switch(type)
	{
	case data_type::CONFIG:
		start_address = start_flash_address;
		end_address = start_flash_address + (max_size::CONFIG);
		if(size > max_size::CONFIG)
		{
			if(error)
			{
				*error = OS_ERROR_BUILD("Data too much bigger than CONFIG area.", error_type::OS_EMSGSIZE);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return {{}, {}};
		}
		break;
	case data_type::DATA:
		start_address = start_flash_address + (max_size::CONFIG);
		end_address   = end_flash_address;
		if(size > max_size::DATA)
		{
			if(error)
			{
				*error = OS_ERROR_BUILD("Data too much bigger than DATA area.", error_type::OS_EMSGSIZE);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return {{}, {}};
		}
		break;
	}

	meta_address start;
	if(start_address >= static_cast<uint32_t>(addr_flash::PAGE_1) && start_address <= static_cast<uint32_t>(addr_flash::PAGE_127))
	{
		start.bank = FLASH_BANK_1;

	}
	else if(start_address >= static_cast<uint32_t>(addr_flash::PAGE_128) && start_address <= static_cast<uint32_t>(addr_flash::PAGE_255))
	{
		start.bank = FLASH_BANK_2;
	}
	start.address = start_address;

	meta_address end;
	if(end_address >= static_cast<uint32_t>(addr_flash::PAGE_1) && end_address <= static_cast<uint32_t>(addr_flash::PAGE_127))
	{
		end.bank = FLASH_BANK_1;

	}
	else if(end_address >= static_cast<uint32_t>(addr_flash::PAGE_128) && end_address <= static_cast<uint32_t>(addr_flash::PAGE_255))
	{
		end.bank = FLASH_BANK_2;
	}
	end.address = end_address;

	return {start, end};
}

} /* namespace driver */
} /* namespace hhg */
