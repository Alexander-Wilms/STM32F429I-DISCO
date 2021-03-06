// finite state machine

#ifndef FSM
#define FSM

typedef enum { RUNNING, HALTED, N_STATES } state_type;

typedef enum { TICK, START_STOP, N_EVENTS } event_type;

int     get_event( void);
void    signal_event( int event);

#define NO_EVENT        0xff
#define NIX             0xff

#endif //FSM
