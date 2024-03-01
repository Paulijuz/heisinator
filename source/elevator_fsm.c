#include <time.h>

#include "elevator_fsm.h"

// Calculate direction
int sign(int x) {
  return (x > 0) - (x < 0);
  // return (x != 0) ? x / abs(x) : 0;

}
int dir(int target, int current) {
  int diff = target - current;
  return sign(diff);
}

void fsm_startup();
void fsm_idle();
void fsm_moving();
void fsm_emergency_stop();

void fsm_set_direction(MotorDirection direction);
void reset_door_timeout(void);
void open_door(void);
void close_door(void);

// Fill array
static state_config_t states[] = {
    {STARTUP,        "Startup",        fsm_startup       },
    {IDLE,           "Idle",           fsm_idle          },
    {MOVING,         "Moving",         fsm_moving        },
    {EMERGENCY_STOP, "Emergency stop", fsm_emergency_stop}
};

// Current state
static states_t       current_state     = STARTUP;

// State variables
static MotorDirection current_direction  = DIRN_STOP;
static MotorDirection previous_direction = DIRN_STOP;
static door_state_t   door_status        = DOOR_CLOSED;
static time_t         door_timeout       = 0;

void elevator_fsm(void) {
    // Emergency stop button
    if (input_stop_button_held()) {
        set_state(EMERGENCY_STOP);
    }

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
    if (get_current_floor() == -1) {
        fsm_set_direction(DIRN_DOWN);
        return;
    }
    
    fsm_set_direction(DIRN_STOP);
    set_state(IDLE);
}
void fsm_idle() {
    log_debug("Order: %d", orders_exists());

    // Check if there are any inputs
    if (orders_exists() && door_status != DOOR_OPEN) {
        set_state(MOVING);
        return;
    }

    if (door_status == DOOR_OPEN && time(NULL) > door_timeout && !input_door_obstruction()) {
        close_door();
    }
    if (input_door_obstruction() && door_status == DOOR_OPEN) {
        reset_door_timeout();
    }
}
void fsm_moving() {
    int last_floor = get_last_floor();
    int current_direction = fsm_get_previous_direction();

    // Calculate next movement direction
    int order_floor = orders_get_floor(last_floor, current_direction);
    if (order_floor != -1) {
        fsm_set_direction(dir(order_floor, last_floor));
    } else {
        // Edge case
        log_error("No orders in queue, but in MOVING state");
        set_state(IDLE);
        return;
    }

    if (last_floor == order_floor) {
        fsm_set_direction(DIRN_STOP);
        
        open_door();
        set_state(IDLE);
        return;
    }
}
void fsm_emergency_stop() {
    // Stop elevator
    fsm_set_direction(DIRN_STOP);

    // Open door
    if (get_current_floor() != -1) {
        open_door();
    }

    // Clear all orders
    orders_clear_all();

    // Wait for stop button to be released
    if (!input_stop_button_held()) {
        if (get_last_floor() == -1) set_state(STARTUP);
        else                        set_state(IDLE);
    }
}

// State API
states_t get_state(void) {
    return states[current_state].state;
}
void set_state(states_t state) {
    current_state = state;
}
int fsm_get_previous_direction(void) {
    return previous_direction;
}

// Door
void fsm_set_direction(MotorDirection direction) {
    if (direction == current_direction) return;
    
    // Update variables
    if (current_direction != DIRN_STOP) previous_direction = current_direction;
    current_direction = direction;

    // Call API
    elevio_motorDirection(direction);
}
void reset_door_timeout(void) {
    door_timeout = time(NULL) + 3;
}
void open_door(void) {
    door_status = DOOR_OPEN;
    reset_door_timeout();
}
void close_door(void) {
    door_status = DOOR_CLOSED;
}

bool get_door_open() {
    return door_status == DOOR_OPEN;
}
