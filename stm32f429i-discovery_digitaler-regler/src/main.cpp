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

int sum = 0;

int pi(int kp, int TN, int T, int ek)
{
    return (kp*(ek+T/TN*sum));
    sum = sum + ek;
}

int ptone(int kp, int T1, int T, int ek, int vkvorher)
{
	return (T1/(T1+T))*vkvorher + kp*(T/(T1+T))*ek;
}

static uint32_t CurrentFrameBuffer = LCD_FRAME_BUFFER;
static uint16_t CurrentTextColor   = 0x0000;

void plot(int x[], int y[])
{
	int end = sizeof(x)/sizeof(x[0]);
	for (int i =0;i<end;i++)
	{
		LCD_DrawLine((uint16_t) x[i], (uint16_t) y[i], (uint16_t) 1, LCD_DIR_HORIZONTAL);
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

	int a[7] = {10,20,30,40,50,60,70};
	int b[7] = {10,20,30,40,50,60,70};

	plot(a,b);

	//uint32_t cursor = LCD_SetCursor(10,20);

	// zeitdiskret
	/*t = 0:0.1:39;
	e = ones(1,391);
	w = ones(1,391);
	y = zeros(1,391);
	x = zeros(1,391);
	b = zeros(1,391);
	x = zeros(1,391);*/

	/*int t[100];
	for(int j = 0;j<100;j++)
	{
		t[j] = j;
	}
	int e[100];
	for(int k = 0;k<100;k++)
	{
		e[k] = 50;
	}
	plot(t,e);*/

	/*int y[100];
	for(int l = 0;l<100;l++)
	{
		y[l] = 0;
	}

	for (int i = 1;i<100;i++)
	{
	    if (i == 1)
	    {
	        y[i] = pi(2,1,10,e[i]);
	    }
	    else
	    {
	        y[i] = pi(2,1,10,e[i]);
	    }
	}

	plot(t,y);*/

	/*% zeitkontinuierlich
	figure()
	gs = 2/(1+s)
	step(gs)
	% zeitdiskret
	for j = 1:length(e)
	    if j == 1
	        x(j) = PT1(2,1,0.1,e(j),0);
	    else
	        x(j) = PT1(2,1,0.1,e(j),x(j-1));
	    end
	end
	hold on
	plot(t,x,'ro',t,e,'gx')

	% zeitkontinuierlich
	figure()
	step((gr*gs)/(1+gr*gs))
	% zeitdiskret
	for k = 1:length(e)-1
	    if k == 1
	        [y(k),mysum] = PI(2,1,0.1,1,0);
	        x(k) = PT1(2,1,0.1,y(k),0);
	        e(k+1) = w(k+1)-x(k);
	    else
	        [y(k),mysum] = PI(2,1,0.1,e(k),mysum);
	        x(k) = PT1(2,1,0.1,y(k),x(k-1));
	        e(k+1) = w(k+1)-x(k);
	    end
	end
	hold on
	%plot(t,x,'c+',t,w,'ro',t,e,'gx',t,y,'b-.')
	plot(t,x,'ro',t,w,'gx')
	legend('x (Soll)','x','w')
	%plot(t,b)
	xlim([0 12])*/



	for(;;);
}
