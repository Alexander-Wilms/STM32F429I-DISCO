/*
 * uart.cpp
 *
 *  Created on: Nov 16, 2015
 *      Author: schaefer
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f429i_discovery.h"
#include "stm32f4xx_hal_uart.h"
#include "uart.h"
#include <string.h>
#include "common.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f4xx_hal.h"


ROM char message[]="hello via UART\r\n";

extern TaskHandle_t LCDTaskHandle;
extern SemaphoreHandle_t xSemaphore;
extern QueueHandle_t xQueue;

/** @brief  demo task demonstrating USART usage */
void uart_task( void *)
{
	BSP_LED_Init (LED3);
	uint8_t buffer[2]={0, 0};
	uart uart3;

	uart3.puts(message);

	/*char a[] = {"test\r\n"};
	uart3.puts(a);*/

	while( true)
    {
		uart3.wait_4_character();
		buffer[0] = uart3.receive();
		uart3.puts( (const char *)buffer);

		// LCD-Task resumen
		// vTaskResume(LCDTaskHandle);

		if( xSemaphore != NULL )
		{
			xSemaphoreGive( xSemaphore );
		}

		if( xQueue != 0 )
		{
			if( xQueueSend( xQueue, (void *) buffer, ( TickType_t ) 10 ) != pdPASS )
			{
				// Failed to post the message, even after 10 ticks.
			}
		}
    }
}
