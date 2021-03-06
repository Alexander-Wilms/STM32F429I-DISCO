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
#include "stm32f429i_discovery_lcd.h"

extern QueueHandle_t DisplayQueue;
extern QueueHandle_t TerminalQueue;
extern SemaphoreHandle_t xSemaphoreMutex;


/** @brief  demo task demonstrating USART usage */
void uart_receive_task( void *)
{
	uart uart3;
	uart3.puts( (const char *) "uart_receive_task");

	uint8_t buffer[2]={0, 0};

	/*int done = 0;
	while( !done)
	{
		if(xSemaphoreTake( xSemaphoreMutex, ( TickType_t ) 0 ) == pdTRUE )
		{
			uart3.puts((const char*) "receive_task\r\n");
			xSemaphoreGive( xSemaphoreMutex );
			done = 1;
		}
	}*/

	while( true)
    {
		uart3.wait_4_character();
		buffer[0] = uart3.receive();
		uart3.puts( (const char *)buffer);

		if( DisplayQueue != 0 )
		{
			if( xQueueSend( DisplayQueue, (void *) buffer, ( TickType_t ) 10 ) != pdPASS )
			{
				// Failed to post the message, even after 10 ticks.
			}
		}
    }
}

void uart_send_task( void *)
{
	uart uart3;
	uart3.puts( (const char *) "uart_send_task");

	/*int done = 0;
	while( !done)
	{
		if(xSemaphoreTake( xSemaphoreMutex, ( TickType_t ) 0 ) == pdTRUE )
		{
			uart3.puts((const char*) "send_task\r\n");
			xSemaphoreGive( xSemaphoreMutex );
			done = 1;
		}
	}*/
	uart3.puts( (const char *)"hello");

	char terminalbuffer[2] = {0,0};

	while( true)
    {
		if( TerminalQueue != 0 )
		{
			while( xQueueReceive( TerminalQueue, &( terminalbuffer[0] ), ( TickType_t ) 0 ) )
			{
				// pcRxedMessage now points to the struct AMessage variable posted
				// by vATask.
				if( xSemaphoreMutex != NULL )
				{
					if( xSemaphoreTake( xSemaphoreMutex, ( TickType_t ) 0 ) == pdTRUE )
					{
							uart3.puts( (const char *)terminalbuffer);
							xSemaphoreGive( xSemaphoreMutex );
					}
				}
			}
		}
    }
}
