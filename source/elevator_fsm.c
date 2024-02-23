#include "elevator_fsm.h"

void fsm_startup();
void fsm_idle();
void fsm_moving();
void fsm_door_open();
void fsm_emergency_stop();

static int last_floor = -1;

// Fill array
state_config_t states[] = {
  {STARTUP,        "Startup",        fsm_startup},
  {IDLE,           "Idle",           fsm_idle},
  {MOVING,         "Moving",         fsm_moving},
  {DOOR_OPEN,      "Door open",      fsm_door_open},
  {EMERGENCY_STOP, "Emergency stop", fsm_emergency_stop}
};

// Current state
states_t current_state = STARTUP;
MotorDirection current_direction = DIRN_STOP;

void elevator_fsm(void) {
  // Run operation for current state
  for (int i = 0; i < sizeof(states) / sizeof(states[0]); i++) {
    if (states[i].state == current_state) {
      states[i].operation();
      break;
    }
  }
}

// State functions
void fsm_startup() {
  // Move elevator down until it reaches a floor
  if (get_last_floor() == -1) {
    if (current_direction != DIRN_DOWN) {
      current_direction = DIRN_DOWN;
      elevio_motorDirection(current_direction);
    }
  } else {
    current_direction = DIRN_STOP;
    elevio_motorDirection(current_direction);
    set_state(IDLE);
  }
}
void fsm_idle() {
  // Check if there are any inputs
  if (orders_length() > 0) {
    set_state(MOVING);
  }
}
void fsm_moving() {}
void fsm_door_open() {}
void fsm_emergency_stop() {}

// State API
states_t get_state(void) {
  return states[current_state].state;
}
void set_state(states_t state) {
  current_state = state;
}

// Set last floorj  
int get_last_floor(void) {
  return last_floor;
}
void set_last_floor(int floor) {
  last_floor = floor;
}
