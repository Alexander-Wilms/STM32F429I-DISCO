#include "stm32f429i_discovery.h" // F�r __HAL_GPIO_EXTI_CLEAR_IT
#include "stm32f429i_discovery_lcd.h"

extern volatile bool LED_is_ON;
extern volatile bool running_state;

volatile int count;

/** @brief  This function handles External line0 interrupt request from KEY
 *
 * It will be called whenever the button is pressed
 * */
extern "C" void
EXTI0_IRQHandler (void)
{
	count = 1000;
	// clear interrupt pending bit
	EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
	// disable interrupt
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	// start/stop clock
	running_state = !running_state;
	// Show status
	STM_EVAL_LEDOn( LED4);
	// Toggle LED
	LED_is_ON = !LED_is_ON;
}

/** @brief button interrupt initialization */
void
pushbutton_init (void)
{
  /* Initialize USER Buttons with interrupt capability */
	// Dr�cken f�hrt zu Interrupt
	STM_EVAL_PBInit (BUTTON_USER, BUTTON_MODE_EXTI);
}
