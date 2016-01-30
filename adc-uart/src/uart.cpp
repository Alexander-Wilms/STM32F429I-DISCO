/*
 * uart.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: schaefer
 */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f429i_discovery.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "lcd.h"
#include "common.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_10
#define USARTx_TX_GPIO_PORT              GPIOC
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_11
#define USARTx_RX_GPIO_PORT              GPIOC
#define USARTx_RX_AF                     GPIO_AF7_USART3

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE                   (COUNTOF(aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                     (COUNTOF(aTxEndMessage) - 1)

/* Size of Reception buffer */
#define RXBUFFERSIZE                      10

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

uint8_t rxchar;

uart::uart ( void)
{
    huart.Instance          = USARTx;
    huart.Init.BaudRate     = 115200;
    huart.Init.WordLength   = UART_WORDLENGTH_8B;
    huart.Init.StopBits     = UART_STOPBITS_1;
    huart.Init.Parity       = UART_PARITY_NONE;
    huart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart.Init.Mode         = UART_MODE_TX_RX;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;

    GPIO_InitTypeDef  GPIO_InitStruct;

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    USARTx_TX_GPIO_CLK_ENABLE();
    USARTx_RX_GPIO_CLK_ENABLE();
    /* Enable USART1 clock */
    USARTx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = USARTx_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct.Alternate = USARTx_TX_AF;

    HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = USARTx_RX_PIN;
    GPIO_InitStruct.Alternate = USARTx_RX_AF;

    HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

    /*##-3- Configure the NVIC for UART ########################################*/
    /* NVIC for USARTx */
    HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USARTx_IRQn);
    if( HAL_UART_Init( &huart) != HAL_OK)
      asm("bkpt 0");

    HAL_StatusTypeDef status;
    // status=HAL_UART_Receive_IT( &huart, (uint8_t *)0x01, 1); // dummy call to switch RX on
    status=HAL_UART_Receive_IT( &huart, (uint8_t *)&rxchar, 1); // dummy call to switch RX on
    if( status != HAL_OK)
        asm("bkpt 0");
}

void uart::puts( const char * data)
{
	//while(HAL_UART_Transmit_IT( &huart, (uint8_t *)data, strlen(data)) != HAL_OK);

	HAL_UART_Transmit_IT( &huart, (uint8_t *)data, strlen(data));
}

void uart::wait_4_character( void)
{
	while( received_char == 0)
		;
}

char uart::receive( void)
{
	char ret = received_char;
	received_char = 0;
	return ret;
}

UART_HandleTypeDef uart::huart;
uint8_t uart::received_char;

/** @brief USART general error callback */
extern "C" void HAL_UART_ErrorCallback( UART_HandleTypeDef *)
{
  // arrive here on USART error
  asm("bkpt 0");
}

/** @brief USART callback on reception */
extern "C" void deliver_RX_character( uint8_t c)
{
	uart::received_char=c;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	deliver_RX_character(rxchar);

	// start the receive process under the RX callback
	// see https://my.st.com/public/STe2ecommunities/mcu/Lists/STM32Java/Flat.aspx?RootFolder=%2Fpublic%2FSTe2ecommunities%2Fmcu%2FLists%2FSTM32Java%2FUART%20forwarding&FolderCTID=0x01200200770978C69A1141439FE559EB459D758000F9A0E3A95BA69146A17C2E80209ADC21&currentviews=1228
	HAL_UART_Receive_IT( huart, (uint8_t *)&rxchar, 1);
}

/** @brief USART3  RX / TX / ERROR interrupt */
extern "C" void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart::huart);
}