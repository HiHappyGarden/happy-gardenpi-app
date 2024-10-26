/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2025 Antonio Salsi <passy.linux@zresa.it>
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

#include "stm32g4xx/driver-lpuart.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_usart.h"

#include <stdlib.h>
#include <stdbool.h>

extern void Error_Handler(void);
extern void osal_us_sleep(uint64_t us);

static driver_lpuart_on_rx_callback on_rx_callback = NULL;
static UART_HandleTypeDef* hlpuart1 = NULL;
static __IO bool tx_busy = false;

uint8_t driver_lpuart_init(void)
{
	  /*## Configure UART peripheral for reception process (using LL) ##########*/
	  /* Any data received will be stored "aRxBuffer" buffer : the number max of
	     data received is RXBUFFERSIZE */
	  /* Enable RXNE and Error interrupts */
	LL_USART_EnableIT_RXNE(LPUART1);
	LL_USART_EnableIT_ERROR(LPUART1);

	return EXIT_SUCCESS;
}

void driver_lpuart_register(UART_HandleTypeDef* _hlpuart1)
{
	hlpuart1 = _hlpuart1;
}

void driver_lpuart_register_rx_callback(driver_lpuart_on_rx_callback _on_rx_callback)
{
	on_rx_callback = _on_rx_callback;
}

uint8_t driver_lpuart_transmit(const uint8_t* data, uint16_t size)
{
	/* Enable TXE interrupt */
	LL_USART_EnableIT_TXE(LPUART1);

	int32_t timer = 250;
	while (tx_busy)
	{
		timer -= 10;
		HAL_Delay(10);
		if(timer <= 0)
		{
			tx_busy = false;
			LL_USART_DisableIT_TXE(LPUART1);
		}
	}
	tx_busy = true;

	if(HAL_UART_Transmit(hlpuart1, data, size, 50) != HAL_OK)
	{
		/* Transfer error in transmission process */
		tx_busy = false;
		LL_USART_DisableIT_TXE(LPUART1);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
  * @brief  Rx Transfer completed callback
  * @retval None
  */
void driver_lpuart_rx_callback(void)
{
	uint8_t b = LL_USART_ReceiveData8(LPUART1);
	if(on_rx_callback)
	{
		on_rx_callback(b);
	}
}


void driver_lpuart_tx_callback(void)
{
	/* Disable TXE interrupt */
	LL_USART_DisableIT_TXE(LPUART1);

	/* Enable TC interrupt */
	LL_USART_EnableIT_TC(LPUART1);
}


void driver_lpuart_tx_complete_callback(void)
{
	/* Disable TC interrupt */
	LL_USART_DisableIT_TC(LPUART1);

	/* Set Tx complete boolean to 1 */
	tx_busy = false;
}

void driver_lpuart_error_callback(void)
{

  NVIC_DisableIRQ(LPUART1_IRQn);


}

