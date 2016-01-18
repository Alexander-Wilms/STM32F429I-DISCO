/**
 * @file        lcd.cpp
 * @brief       simple LCD driver module
 */

#include "stm32f429i_discovery_lcd.h"

#include "fonts.h"

#include <string.h>
#include <string>
#include <sstream>

volatile uint16_t TextColor, BackColor; // globals needed by LCD driver

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
