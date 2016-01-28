/**
  ******************************************************************************
  * @file    systick.cpp
  * @author  Dr. Klaus Schaefer
  * @brief   timer interrupt driver using the SysTick module
  ******************************************************************************
*/

#include <stdint.h>
#include "systick.h"
//#include "cmsis_device.h"
#include "stm32f429xx.h"
#include "stm32f4xx_hal.h"

#define TICK_RATE_HZ 1000       //!< SysTick interrupt frequency

/** @brief SysTick interrupt service route called @ 1000 Hz */
/*extern "C" void SysTick_Handler(void)
{
#if defined(USE_HAL_DRIVER)
  HAL_IncTick();
#endif
}*/

/** @brief SysTick initialization */
void SysTick_init( void)
{
  SysTick_Config (SystemCoreClock / TICK_RATE_HZ);
}
