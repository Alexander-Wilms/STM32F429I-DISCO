#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>

static void SystemClock_Config(void);
static volatile int sum;

#define SCALE 100

double pi_stellungsalg(double kp, double TN, double T, double ek)
{
    double returnvalue = (kp*(ek+(T/TN)*sum));
    sum = sum + ek;
    return returnvalue;
}

double pid_schnelligkeitsalg(double kp, double kd, double ki, double T, double ek, double ekminuseins, double ekminuszwei, double ykminuseins)
{
    return ykminuseins+ek*(kp+kd/T)-ekminuseins*(kp+(2*kd)/T-ki*T)+ekminuszwei*kd/T;
}

double ptone(double kp, double T1, double T, double ek, double vkvorher)
{
	return (T1/(T1+T))*vkvorher + kp*(T/(T1+T))*ek;
}

double pttwo(double kp, double T1, double T, double ek, double vkminus1, double vkminus2)
{
	return (kp*ek-T1*((vkminus2-2*vkminus1)/(T*T))-2*T1*(-vkminus1/T))/(T1/(T*T)+2*T1/T+1);
}

int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(1, (uint32_t) LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();

	// BSP_LCD_DisplayStringAtLine((uint16_t) 1, (uint8_t *) "Hallo");

	/*Point Points[3];
	Points[0].X = 100;
	Points[0].Y = 100;
	Points[1].X = 200;
	Points[1].Y = 200;
	Points[2].X = 100;
	Points[2].Y = 300;
	BSP_LCD_DrawPolygon(Points, 3);*/

	//BSP_LCD_DrawLine(50,50,150,200);

	// Eingangssprung
	double t[320];
	for(int j = 0;j<sizeof(t)/sizeof(t[0]);j++)
	{
		t[j] = j;
	}
	double e[320];
	for(int k = 0;k<sizeof(t)/sizeof(t[0]);k++)
	{
		/*if(k<20)
			e[k] = 0.;
		else*/
			e[k] = SCALE;
	}
	BSP_LCD_SetTextColor(LCD_COLOR_RED);
	for (int i = 1; i<sizeof(t)/sizeof(t[0])-1; i++)
	{
		BSP_LCD_DrawLine(e[i],t[i],e[i+1],t[i+1]);
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
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGREEN);
	for (int i = 1; i<sizeof(t)/sizeof(t[0])-1; i++)
	{
		BSP_LCD_DrawLine(y[i],t[i],y[i+1],t[i+1]);
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
			y[i] = pi_stellungsalg(2.,10.,0.1,e[i]);
		else
			y[i] = pi_stellungsalg(2.,10.,0.1,e[i]);

	}
	BSP_LCD_SetTextColor(LCD_COLOR_ORANGE);
	for (int i = 1; i<sizeof(t)/sizeof(t[0])-1; i++)
	{
		if(y[i]>240)
			y[i] = 240;
		if(y[i+1]>240)
			y[i+1] = 240;
		BSP_LCD_DrawLine(y[i],t[i],y[i+1],t[i+1]);
	}

	double w[320];
	for(int k = 0;k<sizeof(t)/sizeof(t[0]);k++)
	{
		/*if(k<20)
			w[k] = 0.;
		else*/
			w[k] = SCALE;
	}

	double x[320];
	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		x[l] = 0.;
	}

	// double pi_stellungsalg(double kp, double TN, double T, double ek)
	sum = 0;
	for (int i = 0; i<(sizeof(t)/sizeof(t[0]))-1; i++)
	{
		if (i == 1)
		{
			y[i] = pi_stellungsalg(2,10,0.1,1);
			x[i] = ptone(2,1,0.1,y[i],0);
			e[i+1] = w[i+1]-x[i];
		}
		else
		{
			y[i] = pi_stellungsalg(2,10,0.1,e[i]);
			x[i] = ptone(2,1,0.1,y[i],x[i-1]);
			e[i+1] =  w[i+1]-x[i];
		}
	}
	BSP_LCD_SetTextColor(LCD_COLOR_DARKGRAY);
	for (int i = 1; i<sizeof(t)/sizeof(t[0]); i++)
	{
		BSP_LCD_DrawLine(x[i],t[i],x[i+1],t[i+1]);
	}

	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		x[l] = 0.;
	}


	// Ausgang PID-Glied
	for(int l = 0;l<sizeof(t)/sizeof(t[0]);l++)
	{
		y[l] = 0.;
	}

	//double pid_schnelligkeitsalg(double kp, double kd, double ki, double T, double ek, double ekminuseins, double ekminuszwei, double ykminuseins)
	for (int i = 0;i<sizeof(t)/sizeof(t[0]);i++)
	{
		if(i==0)
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,0,0,0,0);
		if(i==1)
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,0,0,0,0);
		else
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,e[i],e[i-1],e[i-2],y[i-1]);

	}
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	for (int i = 1; i<sizeof(t)/sizeof(t[0])-1; i++)
	{
		BSP_LCD_DrawLine(y[i]/10,t[i],y[i+1]/10,t[i+1]);
	}


	// double pid_schnelligkeitsalg(double kp, double kd, double ki, double T, double ek, double ekminuseins, double ekminuszwei, double ykminuseins)
	// double pttwo(double kp, double T1, double T, double ek, double vkminus1, double vkminus2)
	for (int i = 0; i<(sizeof(t)/sizeof(t[0]))-1; i++)
	{
		if (i == 0)
		{
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,0,0,0,0);
			x[i] = pttwo(0.5,0.1,0.1,y[i],0,0);
			e[i+1] = w[i+1]-x[i];
		}
		if (i == 1)
		{
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,0,0,0,0);
			x[i] = pttwo(0.5,0.1,0.1,y[i],0,0);
			e[i+1] = w[i+1]-x[i];
		}
		else
		{
			y[i] = pid_schnelligkeitsalg(1,0.1,2,0.1,e[i],e[i-1],e[i-2],y[i-1]);
			x[i] = pttwo(0.5,0.1,0.1,y[i],x[i-1],x[i-2]);
			e[i+1] =  w[i+1]-x[i];
		}
	}
	BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
	for (int i = 1; i<sizeof(t)/sizeof(t[0])-1; i++)
	{
		if(x[i]>240)
			x[i] = 0;
		BSP_LCD_DrawLine(x[i],t[i],x[i+1],t[i+1]);
	}

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DrawRect(0, 0, 240-1, 320-1);


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
