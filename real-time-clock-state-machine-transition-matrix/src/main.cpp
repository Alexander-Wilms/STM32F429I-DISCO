/**
  ******************************************************************************
   @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "timer.h"
#include "systick.h"
#include "button.h"
#include "fsm.h"
#include <string>
#include <sstream>

volatile int systick_count = 0;
volatile int systick_init_done = 0;
volatile bool button_pressed = 0;
volatile int button_count = 0;
volatile bool LED_is_ON;

void lcd_init(void)
{
	LCD_Init();
	LCD_LayerInit();

	SysTick_init ();

	pushbutton_init();

	/* Enable the LTDC */
	LTDC_Cmd(ENABLE);

	/* Set LCD foreground layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_SetTransparency(0);

	/* Set LCD background layer */
	LCD_SetLayer(LCD_BACKGROUND_LAYER);

	/* LCD display message */
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	LCD_SetTextColor(LCD_COLOR_WHITE);
}


void display_STOPPING(void)
{

	LCD_ClearLine(LCD_LINE_5);
	LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*) "stopping");
}

void display_STARTING(void)
{

	LCD_ClearLine(LCD_LINE_5);
	LCD_DisplayStringLine(LCD_LINE_5,(uint8_t*) "starting");
}

std::stringstream output;
std::string outputstring;
const char * chararray;

Timer mytimerobject(12,34,56);

void timestuff(void)
{
	systick_count++;
	output.str(std::string());
	mytimerobject.setMin(systick_count/100/60);
	mytimerobject.setSec(systick_count/100);
	mytimerobject.setHun(systick_count/1);
	output << "Time " << mytimerobject.printtime();
	outputstring = "";
	outputstring = output.str();
	chararray = "";
	chararray = outputstring.c_str();
	LCD_DisplayStringLine(LCD_LINE_3,(uint8_t*) chararray);
	LCD_ClearLine(LCD_LINE_4);
}

transition_matrix_element transition_matrix[N_EVENTS][N_STATES] =
{
		{{display_STOPPING,HALTED},{display_STARTING,RUNNING}},
		{{timestuff,RUNNING},{0,(state_type)NIX}}
};

state_type state = RUNNING;
transition_matrix_element * tile;
event_type event;

int main(void)
{
	lcd_init();


	// Super loop
	while(1)
	{
		// wait for event
		while( (event=(event_type)get_event()) == NO_EVENT )
			;

		// find correct field within matrix
		tile=&(transition_matrix[event][state]);

		if( tile->target_state !=NIX)
		{
			// perform transition
			state=tile->target_state;
			// perform action
			tile->action();
		}
	}
}
