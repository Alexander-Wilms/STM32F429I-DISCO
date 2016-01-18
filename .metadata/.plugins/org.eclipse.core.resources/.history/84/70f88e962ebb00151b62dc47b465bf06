/**
 * @file        lcd.cpp
 * @brief       simple LCD driver module
 */

#include "stm32f429i_discovery_lcd.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "fonts.h"

#include <string.h>
#include <string>
#include <sstream>

volatile uint16_t TextColor, BackColor; // globals needed by LCD driver

extern TaskHandle_t LCDTaskHandle;
extern SemaphoreHandle_t xSemaphore;
extern QueueHandle_t DisplayQueue;

/** @brief LCD initialization
 *
 * needs to be once called before theLCD screen can be used
 *  */
void lcd_init( void)
{
	// Initialize the LCD
	BSP_LCD_Init();

	BSP_LCD_LayerDefaultInit(1, (uint32_t) LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(1, ENABLE);
	BSP_LCD_SelectLayer(1);

    // Clear the LCD
	BSP_LCD_Clear(LCD_COLOR_BLACK);

    // Set the LCD Back Color
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

    // Set the LCD Text Color
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

	// 34 chars per line with 12 pt
	BSP_LCD_SetFont(&Font12);
}

/** @brief write a single line of text on the LCD */
void lcd_write_line( unsigned line, char * text)
{
	BSP_LCD_DisplayStringAtLine( LINE(line), (uint8_t *)text);
}

void display_task (void *)
{
	static std::stringstream output;
	static std::string outputstring;
	static const char * chararray;
	static char test[34];
	char first;
	static int i = 0;
	static int k = 0;
	static int a = 1;
	static int b = 0;
	static int done = 0;
	int l = 0;
	int line = 0;
	int reset = 0;
	while(true)
	{

		if( DisplayQueue != 0 )
		{
			if( xQueueReceive( DisplayQueue, &( test[i] ), ( TickType_t ) 0 ) )
			{
				// pcRxedMessage now points to the struct AMessage variable posted
				// by vATask.
				if(test[i]=='\r')
				{
					line++;
					for(int i = 0;i <= 34;i++)
						test[i] = ' ';
					reset = 1;
				}
				i++;
				if(reset)i=0;
				reset=0;
				i = i%34;
			}
		}

		BSP_LCD_DisplayStringAtLine (line, (uint8_t *) &test);
		vTaskDelay(500);
	}
}
