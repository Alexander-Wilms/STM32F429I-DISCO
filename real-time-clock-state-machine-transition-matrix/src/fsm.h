// finite state machine

#ifndef FSM
#define FSM

// The elements in the transition matrix must match
// the order of columns and rows below

// Columns of the transition matrix
typedef enum { RUNNING, HALTED, N_STATES } state_type;

// Rows of the transition matrix
typedef enum { START_STOP, TICK, N_EVENTS } event_type;

typedef void (*fptr)(void);

typedef struct
{
	// pointer to a function
	fptr action;
	state_type target_state;
}
transition_matrix_element;

int     get_event( void);
void    signal_event( int event);

#define NO_EVENT        0xff
#define NIX             0xff

#endif //FSM
