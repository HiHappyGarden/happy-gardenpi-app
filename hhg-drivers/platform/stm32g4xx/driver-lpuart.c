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

#include "stm32g4xx/driver-lpuart.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_usart.h"

#include <stdlib.h>
#include <stdbool.h>

extern void Error_Handler(void);
extern void osal_us_sleep(uint64_t us);

static UART_HandleTypeDef* hlpuart1 = NULL;

static __IO bool  ubTxComplete = false;
static __IO bool ubRxComplete = false;



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

uint8_t driver_lpuart_transmit(const uint8_t* data, uint16_t size)
{

	  /*## Start the transmission process (using LL) *##########################*/
	  /* While the UART in reception process, user can transmit data from "aTxStartMessage" buffer */
	  /* Start USART transmission : Will initiate TXE interrupt after TDR register is empty */
	  //LL_USART_TransmitData8(LPUART1, data[uwTxIndex++]);

	  /* Enable TXE interrupt */
//	  LL_USART_EnableIT_TXE(LPUART1);

	  /*## Wait for the end of the transfer ###################################*/
	  /* USART IRQ handler is not anymore routed to HAL_UART_IRQHandler() function
	     and is now based on LL API functions use.
	     Therefore, use of HAL IT based services is no more possible. */
	  /*  Once TX transfer is completed, LED2 will toggle.
	      Then, when RX transfer is completed, LED2 will turn On. */
//	  while (!ubTxComplete)
//	  {
//		  osal_us_sleep(1000);
//	  }

//	  while (!ubRxComplete)
//	  {
//	  }

//	  BSP_LED_On(LED2); /* stop blink and keeps ON */

	  /*## Send the received Buffer ###########################################*/
	  /* Even if use of HAL IT based services is no more possible, use of HAL Polling based services
	     (as Transmit in polling mode) is still possible. */
//	  if(HAL_UART_Transmit(hlpuart1, (uint8_t*)aRxBuffer, DRIVER_LPUART_RXBUFFERSIZE, 1000)!= HAL_OK)
//	  {
//	    /* Transfer error in transmission process */
//	    Error_Handler();
//	  }

	  /*## Send the End Message ###############################################*/
	  //if(HAL_UART_TRANSMIT(HLPUART1, (UINT8_T*)ATXENDMESSAGE, DRIVER_LPUART_RXBUFFERSIZE, 1000)!= HAL_OK)
	  if(HAL_UART_Transmit(hlpuart1, data, size, 10)!= HAL_OK)
	  {
	    /* Transfer error in transmission process */
	    Error_Handler();
	  }

	  return EXIT_SUCCESS;
}

inline void driver_lpuart_register(UART_HandleTypeDef* _hlpuart1)
{
	hlpuart1 = _hlpuart1;
}

/**
  * @brief  Rx Transfer completed callback
  * @note   This example shows a simple way to report end of IT Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void driver_lpuart_rx_callback(void)
{
//  /* Read Received character. RXNE flag is cleared by reading of RDR register */
//  aRxBuffer[uwRxIndex++] = LL_USART_ReceiveData8(LPUART1);
//
//  /* Check if reception is completed (expected nb of bytes has been received) */
//  if (uwRxIndex == DRIVER_LPUART_RXBUFFERSIZE)
//  {
//    /* Set Reception complete boolean to 1 */
//    ubRxComplete = true;
//  }
}

/**
  * @brief  Function called for achieving next TX Byte sending
  * @retval None
  */
void driver_lpuart_tx_callback(void)
{
//  if(uwTxIndex == (ubSizeToSend - 1))
//  {
//    /* Disable TXE interrupt */
//    LL_USART_DisableIT_TXE(LPUART1);
//
//    /* Enable TC interrupt */
//    LL_USART_EnableIT_TC(LPUART1);
//  }
//
//  /* Fill TDR with a new char */
//  LL_USART_TransmitData8(LPUART1, aTxStartMessage[uwTxIndex++]);
}

/**
  * @brief  Function called at completion of last byte transmission
  * @retval None
  */
void driver_lpuart_tx_complete_callback(void)
{
//  if(uwTxIndex == sizeof(aTxStartMessage))
//  {
//    uwTxIndex = 0;
//
//    /* Disable TC interrupt */
//    LL_USART_DisableIT_TC(LPUART1);
//
//    /* Set Tx complete boolean to 1 */
//    ubTxComplete = true;
//  }
}

void driver_lpuart_error_callback(void)
{

  NVIC_DisableIRQ(LPUART1_IRQn);


}

