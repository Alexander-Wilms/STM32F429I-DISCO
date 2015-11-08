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

//#include "button.h"
//#include "stm32f4xx_hal.h"
#include <string>
#include <sstream>

int sum;

int pi(int kp, int TN, int T, int ek)
{
    return (kp*(ek+T/TN*sum));
    sum = sum + ek;
}

int ptone(int kp, int T1, int T, int ek, int vkvorher)
{
	return (T1/(T1+T))*vkvorher + kp*(T/(T1+T))*ek;
}



void plot(int x[], int y[])
{
	int i = sizeof(x)-1;
	while(i != 0)
	{
		LCD_DrawCircle((uint16_t) x[i], (uint16_t) y[i], (uint16_t) 1);
		i--;
	}
}



int main(void)
{
	LCD_Init();
	LCD_LayerInit();
	LTDC_Cmd(ENABLE);
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_SetTransparency(0);
	LCD_SetLayer(LCD_BACKGROUND_LAYER);
	LCD_Clear(LCD_COLOR_BLUE);
	LCD_SetBackColor(LCD_COLOR_BLUE);
	LCD_SetTextColor(LCD_COLOR_WHITE);

	//LCD_DrawCircle((uint16_t) 50, (uint16_t) 50, (uint16_t) 1);
	int t[] = {10, 20, 30};
	int x[] = {100, 200, 300};

	plot(t,x);


	for(;;);
}
