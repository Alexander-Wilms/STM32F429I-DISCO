/**
  ******************************************************************************
  * @file    systick.cpp
  * @author  Dr. Klaus Schaefer
  * @brief   timer interrupt driver using the SysTick module
  ******************************************************************************
*/

#include <stdint.h>
#include "systick.h"
#include "stdint.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include <string>
#include <sstream>
#include "stm32f429i_discovery_lcd.h"

#define TICK_RATE_HZ 1000       //!< SysTick interrupt frequency

/** @brief SysTick interrupt service route called @ 1000 Hz */
extern "C" void SysTick_Handler(void)
{


#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif

  if(running)
	  systick_count++;
}

/** @brief SysTick initialization */
void SysTick_init( void)
{
	SysTick_Config (SystemCoreClock / TICK_RATE_HZ);
	systick_init_done = 1;
}
