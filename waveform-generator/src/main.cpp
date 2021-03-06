#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_dac.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"

#include "waveforms.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

static void SystemClock_Config(void);

#define DAC_DHR12R2_ADDRESS    0x40007414

uint16_t Sine12bit[32] = { 2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095,
        4056, 3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909, 599,
        344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647 };

uint16_t Escalator8bit[2] = { 3185, 3498 };
uint16_t Escalator8bit1 = 0;

/* Private functions ---------------------------------------------------------*/
uint32_t systemTick = 0;
uint32_t lastSysTick = 0;

void MyConfig();
void TDMA(void);

void MyConfig() {
    DAC_ChannelConfTypeDef DAC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_HandleTypeDef TIM_TimeBaseStructure;

    /* DMA1 clock and GPIOA clock enable (to be used with DAC) */
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_TIM1_CLK_ENABLE();

    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
    uint16_t PrescalerValue = (uint16_t)((SystemCoreClock / 2) / 100000) - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    /* TIM1 TRGO selection */
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* DMA1_Stream6 channel7 configuration **************************************/
    DMA_DeInit(DMA1_Stream6);
    DMA_InitStructure.Channel = DMA_Channel_7;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & DAC->DHR12R2;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) & Sine12bit;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = 32;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeriphBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
    /* Enable DMA1_Stream6 */
    DMA_Cmd(DMA1_Stream6, ENABLE);

    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    /* Enable DAC Channel2 */
    DAC_Cmd(DAC_Channel_2, ENABLE);
    /* Enable DMA for DAC Channel2 */
    DAC_DMACmd(DAC_Channel_2, ENABLE);
}

void TDMA(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //was 1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Stream6, DMA_IT_TC | DMA_IT_TE | DMA_IT_FE, ENABLE);
}

void DMA1_Stream6_IRQHandler(void) {
    BSP_LED_Toggle(LED4);
}

float g_ADCValue;

int main(void)
{
	HAL_Init();
	MyConfig();
	/* Configure the system clock */
	SystemClock_Config();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(1, (uint32_t) LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();

	int x[320];
	int o[320];
	int p[320];
	int q[320];

	sine(x,o);
	square(x,p);
	sine(x,q);

	int a[320];
	for(int i = 0;i<320;i++)
		a[i] = 0;
	int i = 0;
	int j = 0;

	while(true)
		;
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

	    __HAL_RCC_PWR_CLK_ENABLE();
	    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	    RCC_OscInitStruct.PLL.PLLM = 8;
	    RCC_OscInitStruct.PLL.PLLN = 288;
	    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	    RCC_OscInitStruct.PLL.PLLQ = 6;
	    HAL_RCC_OscConfig(&RCC_OscInitStruct);

	    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
	    SystemCoreClockUpdate();

	    if (HAL_GetREVID() == 0x1001)
	        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}


