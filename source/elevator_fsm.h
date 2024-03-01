#pragma once

#include "queue.h"

// extern bool operating;
// extern int destination;

// States
typedef enum states_e {
    STARTUP,
    IDLE,
    MOVING,
    EMERGENCY_STOP
} states_t;

typedef enum door_state_e {
    DOOR_OPEN,
    DOOR_CLOSED
} door_state_t;

// Array of states
typedef struct state_config_s {
    states_t state;
    char    *name;
    void (*operation)(void);
} state_config_t;

// API
void     elevator_fsm(void);
states_t get_state(void);
void     set_state(states_t state);
int      fsm_get_previous_direction(void);

bool get_door_open();
