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
#include "stm32f4xx_hal_can.h"

// FreeRTOS can be configured via ./Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOSConfig.h
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <string>
#include <sstream>

#define CANx                            CAN1
#define CANx_CLK_ENABLE()               __HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()

#define CANx_FORCE_RESET()              __HAL_RCC_CAN2_FORCE_RESET()
#define CANx_RELEASE_RESET()            __HAL_RCC_CAN2_RELEASE_RESET()

#define CANx_TX_PIN                    GPIO_PIN_13
#define CANx_TX_GPIO_PORT              GPIOB
#define CANx_TX_AF                     GPIO_AF9_CAN2
#define CANx_RX_PIN                    GPIO_PIN_5
#define CANx_RX_GPIO_PORT              GPIOB
#define CANx_RX_AF                     GPIO_AF9_CAN2

static void SystemClock_Config(void);

int main(void)
{
	HAL_Init();

	SystemClock_Config();

	__HAL_RCC_CAN1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;

	CANx_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin = CANx_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =  CANx_TX_AF;

	HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CANx_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =  CANx_RX_AF;

	HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStruct);

	CAN_HandleTypeDef CanHandle;
	CanHandle.Instance = CANx;

	CanHandle.Init.TTCM = DISABLE;
	CanHandle.Init.ABOM = DISABLE;
	CanHandle.Init.AWUM = DISABLE;
	CanHandle.Init.NART = DISABLE;
	CanHandle.Init.RFLM = DISABLE;
	CanHandle.Init.TXFP = DISABLE;
	CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
	CanHandle.Init.SJW = CAN_SJW_1TQ;
	CanHandle.Init.BS1 = CAN_BS1_14TQ;	// this will give 1Mbit/s @ 168 MHz
	CanHandle.Init.BS2 = CAN_BS2_6TQ;
	CanHandle.Init.Prescaler = 2;

	HAL_CAN_Init(&CanHandle);

	CAN_FilterConfTypeDef  sFilterConfig;
	sFilterConfig.FilterNumber = 14;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.BankNumber = 14;

	HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig);

	uint32_t prioritygroup = NVIC_GetPriorityGrouping();

	NVIC_SetPriority(CAN2_RX0_IRQn, NVIC_EncodePriority(prioritygroup, 15, 0));
	NVIC_EnableIRQ(CAN2_RX0_IRQn);
	NVIC_SetPriority(CAN2_TX_IRQn, NVIC_EncodePriority(prioritygroup, 15, 0));
	NVIC_EnableIRQ(CAN2_TX_IRQn);

	CAN2->IER |= CAN_IT_FMP0; // enable CAN2 FIFO 0 RX interrupt

	CanTxMsgTypeDef TxMess;
	CanHandle.pTxMsg = &TxMess;
	TxMess.StdId = 0x241;
	TxMess.IDE = CAN_ID_STD;
	TxMess.DLC = 0;
	HAL_CAN_Transmit(&CanHandle,10);

	#define LED_TASK_PRIORITY ((1 + tskIDLE_PRIORITY) | portPRIVILEGE_BIT)
	//xTaskCreate( (pdTASK_CODE)pwm_task, 	"led1", 256, 0, LED_TASK_PRIORITY, NULL);
	//xTaskCreate( (pdTASK_CODE)manual_pwm_task, 	"led1", 256, 0, LED_TASK_PRIORITY, NULL);
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
