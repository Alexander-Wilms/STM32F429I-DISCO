/**
  ******************************************************************************
  * @file    systick.h
  * @author  Dr. Klaus Schaefer
  * @brief   header for module button
  ******************************************************************************
*/

void SysTick_init( void);
void busy_delay( unsigned time);
extern volatile int systick_count;
extern volatile int systick_init_done;
extern volatile bool running_state;
