#include <inttypes.h>
#include "fsm.h"

volatile uint16_t recorded_events;

int get_event( void)
{
	register uint16_t bit, event;
	if( recorded_events==0)
		return NO_EVENT;

	// find highest priority event
	for( bit=1, event=0; (bit & recorded_events)==0; )
	{
		bit <<=1;
		++event;
	}

	// switch this bit off
	recorded_events &= ~bit;
	return event;
}

void signal_event( int event)
{
	recorded_events |= (1<<event);
}
