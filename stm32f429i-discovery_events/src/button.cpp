#include "stm32f429i_discovery.h" // F�r __HAL_GPIO_EXTI_CLEAR_IT
#include "stm32f429i_discovery_lcd.h"
#include "fsm.h"
//#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_conf.h"
//#include "stm32f4xx_hal_gpio.h"


extern volatile bool LED_is_ON;
extern volatile int button_count;
extern volatile int button_pressed;
extern volatile bool running;

/** @brief  This function handles External line0 interrupt request from KEY
 *
 * It will be called whenever the button is pressed
 * */
extern "C" void
EXTI0_IRQHandler (void)
{
	//count = 50;
	/* reset both EXTI I/O interrupt latches,
	* as both buttons share the same interrupt
	* and we don't know which one triggered */
	//__HAL_GPIO_EXTI_CLEAR_IT( USER_BUTTON_PIN);
	// Disable interrupts
	// STM_EVAL_PBInit (BUTTON_USER, BUTTON_MODE_GPIO);
	// Toggle state
	//if(key==START_STOP)
		//running = !running;
	// Show status
	// STM_EVAL_LEDOn( LED3);
	// Toggle LED
	// LED_is_ON = ! LED_is_ON;
	// LCD_DisplayStringLine(LCD_LINE_12, (uint8_t*) "Button pressed");
	EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
	signal_event(START_STOP);
}

/** @brief button interrupt initialization */
void
pushbutton_init (void)
{
  /* Initialize USER Buttons with interrupt capability */
	// Dr�cken f�hrt zu Interrupt
	STM_EVAL_PBInit (BUTTON_USER, BUTTON_MODE_EXTI);
}