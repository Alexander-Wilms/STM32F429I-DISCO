/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "timer.h"
#include "systick.h"
#include "button.h"
#include <string>
#include <sstream>

volatile int systick_count = 0;
volatile int systick_init_done = 0;
volatile bool button_pressed = 0;
volatile bool running_state = 1;
volatile int button_count = 0;
volatile bool LED_is_ON;
			
void delay(int counter)
{
	while(counter!=0)
		counter--;
}

int main(void)
{
	LCD_Init();
	LCD_LayerInit();

	SysTick_init ();

	pushbutton_init();

	/* Enable the LTDC */
	LTDC_Cmd(ENABLE);

	/* Set LCD foreground layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_SetTransparency(0);

	/* Set LCD background layer */
	LCD_SetLayer(LCD_BACKGROUND_LAYER);

	/* LCD display message */
	LCD_Clear(LCD_COLOR_BLUE);
	LCD_SetBackColor(LCD_COLOR_BLUE);
	LCD_SetTextColor(LCD_COLOR_WHITE);

	std::stringstream output;
	std::string outputstring;
	const char * chararray;

	std::stringstream outputISR;
	std::string outputstringISR;
	const char * chararrayISR;

	std::stringstream outputButton;
	std::string outputstringButton;
	const char * chararrayButton;

	Timer mytimerobject(12,34,56);

	LCD_DisplayStringLine(LCD_LINE_0,(uint8_t*)"h_da    ");
	LCD_DisplayStringLine(LCD_LINE_1,(uint8_t*)"RZS     ");
	LCD_DisplayStringLine(LCD_LINE_2,(uint8_t*)"WS 15/16     ");


	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);

	// Super loop
	while(1)
	{
		if(running_state)
		{
			output.str(std::string());
			mytimerobject.setMin(systick_count/1000/60);
			mytimerobject.setSec(systick_count/1000);
			mytimerobject.setHun(systick_count/10);
			output << "Time " << mytimerobject.printtime();
			outputstring = "";
			outputstring = output.str();
			chararray = "";
			chararray = outputstring.c_str();
			LCD_DisplayStringLine(LCD_LINE_11, (uint8_t*) chararray);
		}

		if(LED_is_ON)
			STM_EVAL_LEDOn(LED3);
		else
			STM_EVAL_LEDOff(LED3);
	}
}
