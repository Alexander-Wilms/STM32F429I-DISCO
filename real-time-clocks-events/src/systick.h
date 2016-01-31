/**
  ******************************************************************************
  * @file    systick.h
  * @author  Dr. Klaus Schaefer
  * @brief   header for module button
  ******************************************************************************
*/

#include "fsm.h"

void SysTick_init( void);
void busy_delay( unsigned time);
extern volatile int systick_count;
extern volatile state_type mystate;
