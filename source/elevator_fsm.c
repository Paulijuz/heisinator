#include <time.h>

#include "elevator_fsm.h"

void fsm_startup();
void fsm_idle();
void fsm_moving();
void fsm_emergency_stop();

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
    if (get_last_floor() == -1) {
        if (current_direction != DIRN_DOWN) {
            current_direction = DIRN_DOWN;
            elevio_motorDirection(current_direction);
        }
        return;
    }
    
    current_direction = DIRN_STOP;
    elevio_motorDirection(current_direction);
    set_state(IDLE);
}
void fsm_idle() {
    // Check if there are any inputs
    if (orders_length() > 0 && door_status != DOOR_OPEN) {
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
    order_element_t next_order;
    bool peeked = orders_peek(&next_order);

    if (!peeked) {
        log_error("No orders in queue, but in MOVING state");
        set_state(IDLE);
        return;
    }

    if (get_current_floor() == next_order.floor) {
        current_direction = DIRN_STOP;
        elevio_motorDirection(current_direction);
        open_door();
        orders_pop(&next_order);
        set_state(IDLE);
        return;
    }
    
    // Edge case when elevator is between floors, and the next order is to the previous floor
    if (get_current_floor() == -1 && get_last_floor() == next_order.floor) {
        if (previous_direction == DIRN_STOP) {
            log_error("Elevator is between floors, and the next order is to the previous floor, but previous direction is DIRN_STOP");
            elevio_motorDirection(DIRN_DOWN);
            return;
        }
        
        current_direction = -previous_direction;
        elevio_motorDirection((MotorDirection) current_direction);
        return;
    }

    // General case
    current_direction = dir(next_order.floor, get_last_floor());
    elevio_motorDirection((MotorDirection) current_direction);
}
void fsm_emergency_stop() {
    // Stop elevator
    elevio_motorDirection(DIRN_STOP);

    // Set previous direction
    previous_direction = current_direction;
    current_direction = DIRN_STOP;

    // Open door
    if (get_current_floor() != -1) {
        open_door();
    }

    // Clear all orders
    orders_clear();

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

// Door
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
