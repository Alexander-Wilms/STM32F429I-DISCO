// finite state machine
// based on http://skript.eit.h-da.de/rzs/Documentation/Rolltor/fsm_8c-source.html

#include <inttypes.h>
#include "fsm.h"
#include <queue>

//std::queue<int> recorded_events;
int recorded_events;

int get_event(void)
{
	//int returnvalue = recorded_events.front();
	//recorded_events.pop();
	//return returnvalue;
	int returnvalue = recorded_events;
	recorded_events = 0xff;
	return returnvalue;
}

void signal_event( int event)
{
	//recorded_events.push(event);
	recorded_events=event;
}
