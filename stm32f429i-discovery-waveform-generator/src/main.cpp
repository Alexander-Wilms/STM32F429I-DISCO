#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_dac.h"

#include <string>
#include <sstream>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265
#define WIDTH 320
#define HEIGHT 240

static void SystemClock_Config(void);

	void sine(int * x, int * y)
	{
		for(int i = 0;i<WIDTH;i++)
		{
			x[i]=i;
			y[i]=sin(i*2*PI/WIDTH)*0.4*HEIGHT+HEIGHT/2;
		}
	}

	void square(int * x, int * y)
	{
		bool high = 1;
		for(int i = 0;i<WIDTH;i++)
		{
			x[i]=i;
			if(high)
				y[i]=0.4*HEIGHT+HEIGHT/2;
			else
				y[i]=HEIGHT/2;
			if(i%(WIDTH/2)==0)
				high = !high;
		}
	}

	void sawtooth(int * x, int * y)
	{
	bool high = 1;
	float j = 0;
	for(int i = 0;i<WIDTH;i++)
	{
		x[i]=i;
		if(high)
		{
			y[i]=(0.4*HEIGHT)*j+HEIGHT/2;
			j += 1/(0.4*HEIGHT);
		}
		else
			{
			y[i]=0+HEIGHT/2;
			j=0;
			high = !high;
		}
		if(i!=0 && i%(WIDTH/4)==0)
			high = !high;
	}
	}


/*int main(void)
{
	HAL_Init();

	/* Configure the system clock */
	/*SystemClock_Config();

	BSP_LCD_Init();
	BSP_LCD_LayerDefaultInit(1, (uint32_t) LCD_FRAME_BUFFER);
	BSP_LCD_SetLayerVisible(1, ENABLE);

	BSP_LCD_SelectLayer(1);
	BSP_LCD_Clear(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayOn();

	int x[320];
	int y[320];

	sine(x,y);

	BSP_LCD_DrawPixel(y[100],x[100],LCD_COLOR_BLACK);

	GPIO_InitTypeDef  GPIO_InitStructure;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Config des DAC-Pins als Analog-Ausgang
	GPIO_InitStructure.Pin = GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	DAC_ChannelConfTypeDef  DAC_InitStructure;
	DAC_HandleTypeDef hdac;

	__HAL_RCC_DAC_CLK_ENABLE();

	// DAC-Config
	DAC_InitStructure.DAC_Trigger=DAC_TRIGGER_NONE;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE ;
	HAL_DAC_Init(&hdac);

	// DAC enable
	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

	while(true)
	{
		for(int i = 0;i<WIDTH;i++)
		{
			HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, y[0]);
			BSP_LCD_DrawPixel(y[i],x[i],LCD_COLOR_BLACK);
		}
	}*/
//}*/


// STM32F4-Discovery Dual DAC 6.25 KHz Sine - sourcer32@gmail.com


/**************************************************************************/

const uint16_t Sine12bit[32] = {
  2047, 2447, 2831, 3185,
  3498, 3750, 3939, 4056,
  4095, 4056, 3939, 3750,
  3495, 3185, 2831, 2447,
  2047, 1647, 1263,  909,
   599,  344,  155,   38,
     0,   38,  155,  344,
   599,  909, 1263, 1647};

uint16_t SineTable[64];

/**************************************************************************/

void RCC_Config(void)
{
  /* DMA1 clock and GPIOA clock enable (to be used with DAC) */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

  /* DAC and TIM6 Peripheral clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC | RCC_APB1Periph_TIM6, ENABLE);
}

/**************************************************************************/

void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**************************************************************************/

void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    int Period;

  Period = ((SystemCoreClock / 2) / 200000); // 200 KHz timebase, 6.25 KHz Sine

  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = Period - 1;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

/**************************************************************************/

#define DAC_DHR12RD_ADDRESS 0x40007428 // Right Align 12-bit Dual

void DAC_Config(void)
{
    DAC_InitTypeDef DAC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  /* DAC channel 1 and 2 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);
  DAC_Init(DAC_Channel_2, &DAC_InitStructure);

  /* DMA1_Stream5 channel7 configuration **************************************/
  DMA_DeInit(DMA1_Stream5);
  DMA_InitStructure.DMA_Channel = DMA_Channel_7;
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12RD_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&SineTable[0];
  DMA_InitStructure.DMA_BufferSize = 32; // 16-bit x 64 / 2
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32-bit 16x2
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream5, &DMA_InitStructure);

  /* Enable DMA1_Stream5 */
  DMA_Cmd(DMA1_Stream5, ENABLE);

  /* Enable DAC Channel 1 and 2 */
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_Cmd(DAC_Channel_2, ENABLE);

  /* Enable DMA for DAC Channel1 */
  DAC_DMACmd(DAC_Channel_1, ENABLE);
}

/**************************************************************************/

int main(void)
{
  int i, j;

  j = 0;

  for(i=0; i<32; i++)
  {
    SineTable[j++] = Sine12bit[i];
    SineTable[j++] = Sine12bit[(i + 8) % 32]; // 90 degree phase shift on Channel 2
  }

  RCC_Config();

  GPIO_Config();

  TIM6_Config();

  DAC_Config();

  while(1); // Do not exit
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

