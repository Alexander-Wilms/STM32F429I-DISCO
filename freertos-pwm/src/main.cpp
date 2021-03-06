/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <string.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_io.h"
#include "stm32f429i_discovery_ts.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"

// FreeRTOS can be configured via ./Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOSConfig.h
#include "FreeRTOS.h"
#include "task.h"

#include <string>
#include <sstream>

#define PERIOD 255 //255

static void SystemClock_Config(void);

void manual_pwm_task (void *)
{
	BSP_LED_Init (LED3);
	BSP_LED_Init (LED4);
	int i, j = 0;
	int total = 100000;
	float duty = 0.5;
	while (true)
	{
		BSP_LED_On(LED3);
		BSP_LED_On(LED4);
		for(i=0;i<total*duty;i++);
		BSP_LED_Off(LED3);
		BSP_LED_Off(LED4);
		for(i=0;i<total*(1-duty);i++);
		// LED
		duty = (sin(j*3.1415/180)+1)/2;
		// Motor
		//duty = 0.5+(sin(j*3.1415/180)+1)/4;
		j++;
	}
}

void pwm_task (void *)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_HandleTypeDef TimHandle;
	TIM_OC_InitTypeDef PWMConfig;
	TIM_BreakDeadTimeConfigTypeDef DeadConfig;

	__GPIOB_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();

	// Actual PWM can only use specific pins (not the ones connected to the LEDs)
	// Connect pins PB6 and PD12 to PG13, with just one wire it's quite dim
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	__TIM4_CLK_ENABLE();

	TimHandle.Instance = TIM4;
	TimHandle.Init.Period = PERIOD;
	TimHandle.Init.Prescaler = 0;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;

	HAL_TIM_PWM_Init(&TimHandle);

	PWMConfig.Pulse = PERIOD-1;
	PWMConfig.OCMode = TIM_OCMODE_PWM1;
	PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	PWMConfig.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	PWMConfig.OCIdleState = TIM_OCIDLESTATE_SET;
	PWMConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	PWMConfig.OCFastMode = TIM_OCFAST_DISABLE;

	HAL_TIM_PWM_ConfigChannel(&TimHandle,&PWMConfig,TIM_CHANNEL_1);

	DeadConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
	DeadConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
	DeadConfig.BreakState = TIM_BREAK_DISABLE;
	DeadConfig.DeadTime = 0xff;
	DeadConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	DeadConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	DeadConfig.OffStateRunMode = TIM_OSSR_DISABLE;

	HAL_TIM_PWM_Start(&TimHandle,TIM_CHANNEL_1);

	BSP_LED_Init (LED3);
	BSP_LED_Init (LED4);
	int j = 0;
	float duty = 0.5;
	while (true)
	{
		duty = (PERIOD-1) * (sin(j/10000)+1)/2;
		j++;

		TIM4->CCR1 = duty;
	}
}

int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	#define LED_TASK_PRIORITY ((1 + tskIDLE_PRIORITY) | portPRIVILEGE_BIT)
	//xTaskCreate( (pdTASK_CODE)pwm_task, 	"led1", 256, 0, LED_TASK_PRIORITY, NULL);
	xTaskCreate( (pdTASK_CODE)manual_pwm_task, 	"led1", 256, 0, LED_TASK_PRIORITY, NULL);
	vTaskStartScheduler ();

	return 0;
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
