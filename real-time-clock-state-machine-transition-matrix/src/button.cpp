#include "stm32f429i_discovery.h" // F�r __HAL_GPIO_EXTI_CLEAR_IT
#include "stm32f429i_discovery_lcd.h"
#include "fsm.h"

/** @brief  This function handles External line0 interrupt request from KEY
 *
 * It will be called whenever the button is pressed
 * */
extern "C" void
EXTI0_IRQHandler (void)
{
	EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
	signal_event(START_STOP);
}

/** @brief button interrupt initialization */
void
pushbutton_init (void)
{
  /* Initialize USER Buttons with interrupt capability */
	// Pressing the button causes and interrupt
	STM_EVAL_PBInit (BUTTON_USER, BUTTON_MODE_EXTI);
}
