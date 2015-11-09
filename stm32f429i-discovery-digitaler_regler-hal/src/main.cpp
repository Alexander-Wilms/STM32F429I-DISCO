#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

//#include "button.h"
//#include "stm32f4xx_hal.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>

static void SystemClock_Config(void);
static volatile int sum;

//y[i] = pi(2.,10.,0.1,e[i]);
double pi(double kp, double TN, double T, double ek)
{
    double returnvalue = (kp*(ek+(T/TN)*sum));
    sum = sum + ek;
    return returnvalue;
}

double ptone(double kp, double T1, double T, double ek, double vkvorher)
{
	return (T1/(T1+T))*vkvorher + kp*(T/(T1+T))*ek;
}

void plot(int x[], int y[])
{
	for (int i = 0; i<sizeof(x)/sizeof(x[0]); i++)
	{
		BSP_LCD_DrawPixel((uint16_t) x[i], (uint16_t) y[i], (uint16_t) LCD_COLOR_BLACK);
	}
}

int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(0, (uint32_t) LCD_FRAME_BUFFER);
	// BSP_LCD_SetLayerWindow(0, 0, 0, 240, 160);
	BSP_LCD_SetLayerVisible(0, ENABLE);

	BSP_LCD_SelectLayer(0);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();

	//int a[] = {10,11,12,13,14,15};
	//int b[] = {10,11,12,13,14,15};
	//plot(a,b);

	/*for (int i = 0; i<sizeof(a)/sizeof(a[0]); i++)
	{
		BSP_LCD_DrawPixel((uint16_t) a[i], (uint16_t) b[i], (uint16_t) LCD_COLOR_BLACK);
	}*/

	// Eingangssprung
	double t[320];
	for(int j = 0;j<sizeof(t)/sizeof(t[0]);j++)
	{
		t[j] = j;
	}
	double e[320];
	for(int k = 0;k<sizeof(t)/sizeof(t[0]);k++)
	{
		if(k<20)
			e[k] = 0.;
		else
			e[k] = 50.;
	}
	for (int i = 0; i<sizeof(t)/sizeof(t[0]); i++)
	{
		BSP_LCD_DrawPixel((uint16_t) e[i], (uint16_t) t[i], (uint16_t) LCD_COLOR_GREEN);
	}

	// Ausgang PT1-Glied
	double y[320];
	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		y[l] = 0.;
	}

	sum = 0;
	for (int i = 1;i<sizeof(t)/sizeof(t[0]);i++)
	{
		if(i==1)
			y[i] = ptone(2.,1.,0.1,e[i],0);
		else
			y[i] = ptone(2.,1.,0.1,e[i],y[i-1]);

	}

	for (int i = 0; i<sizeof(t)/sizeof(t[0]); i++)
	{
		BSP_LCD_DrawPixel((uint16_t) y[i], (uint16_t) t[i], (uint16_t) LCD_COLOR_GREEN);
	}

	// Ausgang PI-Glied
	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		y[l] = 0.;
	}

	sum = 0;
	for (int i = 1;i<sizeof(t)/sizeof(t[0]);i++)
	{
		if(i==1)
			y[i] = pi(2.,10.,0.1,e[i]);
		else
			y[i] = pi(2.,10.,0.1,e[i]);

	}

	for (int i = 0; i<sizeof(t)/sizeof(t[0]); i++)
	{
		if(y[i]>240)
			y[i] = 240;
		BSP_LCD_DrawPixel((uint16_t) y[i], (uint16_t) t[i], (uint16_t) LCD_COLOR_GREEN);
	}

	double w[320];
	for(int k = 0;k<sizeof(t)/sizeof(t[0]);k++)
	{
		if(k<20)
			w[k] = 0.;
		else
			w[k] = 50.;
	}

	double x[320];
	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		x[l] = 0.;
	}

	sum = 0;
	for (int i = 0; i<(sizeof(t)/sizeof(t[0]))-1; i++)
	{
		if (i == 1)
		{
			y[i] = pi(2,10,0.1,1);
			x[i] = ptone(2,1,0.1,y[i],0);
			e[i+1] = w[i+1]-x[i];
		}
		else
		{
			y[i] = pi(2,10,0.1,e[i]);
			x[i] = ptone(2,1,0.1,y[i],x[i-1]);
			e[i+1] =  w[i+1]-x[i];
		}
	}

	for (int i = 0; i<sizeof(t)/sizeof(t[0]); i++)
	{
		BSP_LCD_DrawPixel((uint16_t) x[i], (uint16_t) t[i], (uint16_t) LCD_COLOR_GREEN);
	}

	for(;;);
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 *         The LTDC Clock is configured as follow :
 *            PLLSAIN                        = 192
 *            PLLSAIR                        = 4
 *            PLLSAIDivR                     = 8
 * @param  None
 * @retval None
 *
 * COPYRIGHT(c) 2014 STMicroelectronics
 */
static void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/*##-1- System Clock Configuration #########################################*/
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 360;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	HAL_PWREx_ActivateOverDrive();

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	/*##-2- LTDC Clock Configuration ###########################################*/
	/* LCD clock configuration */
	/* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
	/* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
	/* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/4 = 48 Mhz */
	/* LTDC clock frequency = PLLLCDCLK / RCC_PLLSAIDIVR_8 = 48/8 = 6 Mhz */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
	PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
	PeriphClkInitStruct.PLLSAI.PLLSAIR = 4;
	PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_8;
	HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
}
