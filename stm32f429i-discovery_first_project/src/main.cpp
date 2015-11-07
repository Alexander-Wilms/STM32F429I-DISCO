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
//#include "button.h"
//#include "stm32f4xx_hal.h"
#include <string>
#include <sstream>

volatile int mytimer;
volatile int b;
volatile int systick_init_done = 0;
volatile int button_pressed = 0;
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

	//HAL_Init();

	/* LTDC reload configuration */
	//LTDC_ReloadConfig(LTDC_IMReload);

	/* Enable the LTDC */
	LTDC_Cmd(ENABLE);

	/* Set LCD foreground layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_SetTransparency(0);

	/* Set LCD background layer */
	LCD_SetLayer(LCD_BACKGROUND_LAYER);

	/* LCD display message */
	LCD_Clear(LCD_COLOR_WHITE);
	//LCD_SetTextColor(LCD_COLOR_WHITE);

	std::stringstream output;
	std::string outputstring;
	const char * chararray;

	std::stringstream outputISR;
	std::string outputstringISR;
	const char * chararrayISR;

	Timer mytimerobject(12,34,56);

	LCD_DisplayStringLine(LCD_LINE_2,(uint8_t*)"h_da    ");
	LCD_DisplayStringLine(LCD_LINE_4,(uint8_t*)"RZS     ");
	LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"WS 15/16     ");


	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	while(1)
	{
		if((mytimer/1000)%2)
		{
			STM_EVAL_LEDOn(LED3);
		} else
		{
			STM_EVAL_LEDOff(LED3);
		}

		if(LED_is_ON)
		{
			STM_EVAL_LEDOn(LED4);
		} else
		{
			STM_EVAL_LEDOff(LED4);
		}

		output.str(std::string());
		mytimerobject.setMin(mytimer/1000/60);
		mytimerobject.setSec(mytimer/1000);
		mytimerobject.setHun(mytimer/10);
		output << "Time: " << mytimerobject.printtime();
		outputstring = "";
		outputstring = output.str();
		chararray = "";
		chararray = outputstring.c_str();

		LCD_DisplayStringLine(LCD_LINE_9, (uint8_t*) chararray);

		outputISR.str(std::string());
		outputISR << "ISR call " << b;
		outputstringISR = "";
		outputstringISR = outputISR.str();
		chararrayISR = "";
		chararrayISR = outputstringISR.c_str();
		LCD_DisplayStringLine(LCD_LINE_10,(uint8_t*) chararrayISR);

		if(systick_init_done)
			LCD_DisplayStringLine(LCD_LINE_11,(uint8_t*) "SysTick_init ok");

		if(button_pressed)
					LCD_DisplayStringLine(LCD_LINE_12,(uint8_t*) "button pressed");
	}
}
