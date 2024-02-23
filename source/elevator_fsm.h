#include "queue.h"

// extern bool operating;
// extern int destination;

// States
typedef enum states_e {
  STARTUP,
  IDLE,
  MOVING,
  DOOR_OPEN,
  EMERGENCY_STOP
} states_t;

// Array of states
typedef struct state_config_s {
  states_t state;
  char *name;
  void (*operation)(void);
} state_config_t;

// API
void     elevator_fsm(void);
states_t get_state(void);
void     set_state(states_t state);

// Operations
int  get_last_floor(void);
void set_last_floor(int floor);
