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
using namespace hhg::iface;
using namespace os;





#define DATA_32                     ((uint32_t)0x12345678)
#define DATA_64                     ((uint64_t)0x5f5f6369616f5f5f)


namespace hhg::driver
{
namespace v1
{

namespace
{
enum class test_status {FAILED = 0, PASSED = !FAILED} ;

__IO test_status MemoryProgramStatus = test_status::PASSED;

}


stm32_fsio::stm32_fsio(uint32_t start_flash_address, uint32_t end_flash_address) OS_NOEXCEPT
: start_flash_address(start_flash_address)
, end_flash_address(end_flash_address)
{

}

stm32_fsio::~stm32_fsio() = default;

os::exit stm32_fsio::init(error** error) OS_NOEXCEPT
{
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Clear OPTVERR bit set on virgin samples */
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

	/* Unlock the Options Bytes *************************************************/
	HAL_FLASH_OB_Unlock();

	/* Get the number of the start and end pages */
	start_page = get_page(start_flash_address);
	end_page   = get_page(end_flash_address);

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

	uint32_t address = 0;
	uint32_t page_error = 0;

	/* The selected pages are write protected *******************************/
	if (((options_bytes_struct1.WRPStartOffset  <= start_page) && (options_bytes_struct1.WRPEndOffset  >= end_page)) ||
	   ((options_bytes_struct2.WRPStartOffset <= start_page) && (options_bytes_struct2.WRPEndOffset >= end_page)))
	{
		/* The desired pages are write protected */
		/* Check that it is not allowed to write in this page */
		address = start_flash_address;
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, DATA_64) != HAL_OK)
		{
			/* Error returned during programming. */
			/* Check that WRPERR flag is well set */
			if ((HAL_FLASH_GetError() & HAL_FLASH_ERROR_WRP) != 0)
			{
				MemoryProgramStatus = test_status::FAILED;
			}
			else
			{
				/* Another error occurred.
				User can add here some code to deal with this error */
				if(error)
				{
					*error = OS_ERROR_BUILD("Another error occurred.", error_type::OS_EIO);
					OS_ERROR_PTR_SET_POSITION(*error);
				}
				return exit::KO;
			}
		}
		else
		{
			/* Write operation is successful. Should not occur
			User can add here some code to deal with this error */
			if(error)
			{
				*error = OS_ERROR_BUILD("Write operation is successful. Should not occur.", error_type::OS_EIO);
				OS_ERROR_PTR_SET_POSITION(*error);
			}
			return exit::KO;
		}
	}
	else
	{
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

		/* FLASH Word program of DATA_32 at addresses defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR */
		address = start_flash_address;
		while (address < end_flash_address)
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, DATA_64) == HAL_OK)
			{
				address = address + 8;
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

		/* Check the correctness of written data */
		address = start_flash_address;

		while (address < end_flash_address)
		{
			if((*(__IO uint32_t*) address) != DATA_32)
			{
				MemoryProgramStatus = test_status::FAILED;
			}
			address += 4;
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	  to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

	/*Check if there is an issue to program data*/
	if (MemoryProgramStatus == test_status::PASSED)
	{
	 /* No error detected. Switch on LED2*/
	   return exit::OK;
	}
	else
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("Fail to check if there is an issue to program data", error_type::OS_EIO);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}


	return exit::OK;
}

os::exit stm32_fsio::read(data_type type, uint8_t data[], size_t size, error** error) const OS_NOEXCEPT
{

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

} /* namespace driver */
} /* namespace hhg */
