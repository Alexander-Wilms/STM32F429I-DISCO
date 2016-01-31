// finite state machine

#ifndef FSM
#define FSM

typedef enum { RUNNING, HALTED, N_STATES } state_type;

typedef enum { START_STOP, TICK, N_EVENTS } event_type;

typedef void (*fptr)(void);

typedef struct
        {
        fptr action;
        state_type target_state;
        }
transition_matrix_element;

int     get_event( void);
void    signal_event( int event);

#define NO_EVENT        0xff
#define NIX             0xff

#endif //FSM