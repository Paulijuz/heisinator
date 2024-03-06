/**
 * @file elevator_fsm.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief FSM for the elevator
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* ANSI C libraries                                                          */
/*****************************************************************************/
#include <time.h>

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "queue.h"
#include "elevator_fsm.h"

/*****************************************************************************/
/* Local function declarations                                               */
/*****************************************************************************/
int  sign(int x);
int  dir(int target, int current);

void fsm_startup();
void fsm_stopped();
void fsm_moving();
void fsm_emergency_stop();

void fsm_set_direction(MotorDirection direction);
void reset_door_timeout(void);
void open_door(void);
void close_door(void);

/*****************************************************************************/
/* Variable declarations                                                     */
/*****************************************************************************/
// Array of states
static state_config_t states[] = {
  {STARTUP,        "Startup",        fsm_startup       },
  {STOPPED,        "Stopped",        fsm_stopped       },
  {MOVING,         "Moving",         fsm_moving        },
  {EMERGENCY_STOP, "Emergency stop", fsm_emergency_stop}
};

// Current state
static states_t current_state  = STARTUP;
static states_t previous_state = -1;

// State variables
static MotorDirection current_direction   = DIRN_STOP; // can be stopped
static MotorDirection movment_direction   = DIRN_STOP; // should never be stoppet (except at startup)
static MotorDirection departure_direction = DIRN_STOP; // like movment_direction, but only updated when stopped at a floor
static door_state_t   door_status         = DOOR_CLOSED;
static time_t         door_timeout        = 0;

/*****************************************************************************/
/* Function definitions                                                      */
/*****************************************************************************/
/**
 * @brief Main function for the elevator state machine
 */
void elevator_fsm(void) {
    // Emergency stop button
    if (input_stop_button_held()) {
        set_state(EMERGENCY_STOP);
    }

    // Run operation for current state
    for (int i = 0; i < sizeof(states) / sizeof(states[0]); i++) {
        if (states[i].state == current_state) {
            if (previous_state != current_state) {
                previous_state = current_state;

                if (current_state == EMERGENCY_STOP) {
                    log_warning("Entered state: %s", states[i].name);
                } else {
                    log_info("Entered state: %s", states[i].name);
                }
            }

            states[i].operation();
            break;
        }
    }
}

// Declare a group of functions
/**
 * @name State operation
 */
///@{

/**
 * @brief Startup state operation for the elevator
 */
void fsm_startup() {
    // Move elevator down until it reaches a floor
    if (get_current_floor() == -1) {
        fsm_set_direction(DIRN_DOWN);
        return;
    }

    // When a floor has been reached elevator is in a known state and ready to start.
    fsm_set_direction(DIRN_STOP);
    set_state(STOPPED);
}

/**
 * @brief Stopped state operation for the elevator
 */
void fsm_stopped() {
    // Check if there are any orders and door is closed.
    if (queue_any_orders() && door_status != DOOR_OPEN) {
        set_state(MOVING);
        return;
    }

    // Close the door if door time out has been reached.
    if (door_status == DOOR_OPEN && time(NULL) > door_timeout && !input_door_obstruction()) {
        close_door();
    }

    // Reset door timeout if something keeps the door open.
    // This can be either door obstruction or door open button.
    if (input_door_obstruction() && door_status == DOOR_OPEN) {
        reset_door_timeout();
    }
}

/**
 * @brief Moving state operation for the elevator
 */
void fsm_moving() {
    int last_floor    = get_last_floor();
    int current_floor = get_current_floor();
    int at_last_floor = (current_floor == last_floor);

    // Calculate next movement direction
    int order_floor = queue_get_direction(last_floor, at_last_floor, movment_direction);

    LOG_INT(order_floor)

    // Log error if something that should happen happens
    if (order_floor < 0) {
        log_error("No orders in queue, but in MOVING state");
        set_state(STOPPED);
        return;
    }

    // Set the movment direction based on the order. We also need to handle
    // the edgecase where if the elevator was stopped between floors and the
    // next order is back to the floor where i came from then it needs to
    // move in the direction it was travelling before it stopped.
    int direction = (order_floor == last_floor && !at_last_floor)
                      ? -departure_direction
                      : dir(order_floor, last_floor);

    fsm_set_direction(direction);

    // Stop at ordered floor
    if (current_floor == order_floor) {
        fsm_set_direction(DIRN_STOP);
        queue_clear_floor(current_floor);

        open_door();
        set_state(STOPPED);
        return;
    }
}

/**
 * @brief Emergency stop state operation for the elevator
 */
void fsm_emergency_stop() {
    // Stop elevator
    fsm_set_direction(DIRN_STOP);

    // Open door if at a floor
    if (get_current_floor() != -1) {
        open_door();
    }

    // Clear all orders
    queue_clear_all();

    // Wait for stop button to be released
    if (!input_stop_button_held()) {
        if (get_last_floor() == -1)
            set_state(STARTUP);
        else
            set_state(STOPPED);
    }
}
///@} Close group of functions

/**
 * @brief Gets current state of the elevator FSM
 *
 * @return Enum representing the current state
 */
states_t get_state(void) {
    return states[current_state].state;
}

/**
 * @brief Sets the state of the elevator FSM
 *
 * @param[in] state Enum representing the state to set
 */
void set_state(states_t state) {
    current_state = state;
}

/**
 * @brief Sets the direction of the elevator
 *
 * @param[in] direction Enum representing the direction to set
 */
void fsm_set_direction(MotorDirection direction) {
    if (direction == current_direction)
        return;

    // Update variables
    if (direction != DIRN_STOP && get_current_floor() != -1)
        departure_direction = direction;
    if (direction != DIRN_STOP)
        movment_direction = direction;
    current_direction = direction;

    log_debug("Moving in direction: %d (%d) [%d]", current_direction, movment_direction, departure_direction);

    // Call API
    elevio_motorDirection(direction);
}

/**
 * @brief Returns the sign of a number
 *
 * @param[in] x The number to get the sign of

 * @return sign of the number
 */
int sign(int x) {
    return (x > 0) - (x < 0);
}

/**
 * @brief Calculates the relative direction between two floors
 *
 * @param[in] target The target floor
 * @param[in] current The current floor
 *
 * @return Relative direction between the two floors
 */
int dir(int target, int current) {
    int diff = target - current;
    return sign(diff);
}

/** @name Door functions */
///@{

/**
 * @brief Opens the door
 */
void open_door(void) {
    door_status = DOOR_OPEN;
    reset_door_timeout();
}

/**
 * @brief Closes the door
 */
void close_door(void) {
    door_status = DOOR_CLOSED;
}

/**
 * @brief Resets the door timeout
 */
void reset_door_timeout(void) {
    door_timeout = time(NULL) + 3;
}

/**
 * @brief Gets the status of the door
 *
 * @return True if the door is open, false if it is closed
 */
bool get_door_open() {
    return (door_status == DOOR_OPEN);
}
