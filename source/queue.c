#include "queue.h"

#define DIR_TO_BTN(x) x == 0 ? BUTTON_CAB : (x > 0 ? BUTTON_HALL_UP : BUTTON_HALL_DOWN) 

int orders_check_upward(int floor);
int orders_check_downward(int floor);

// Array of orders
bool orders[N_FLOORS][N_BUTTONS] = {0}; 

/**
 * Checks if a given order exists.
 * 
 * @param floor - Floor of order (0-indexed)
 * @param direction - Direction of order (-1, 0, or 1)
*/
bool orders_floor_exists(int floor, int direction) {
    for (int button = 0; button < N_BUTTONS; button++) {
        if (orders[floor][DIR_TO_BTN(direction)] || orders[floor][BUTTON_CAB]) {
            return true;
        }
    }
    return false;
}
bool orders_any_exist() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (orders[floor][button]) {
                return true;
            }
        }
    }
    return false;
}

void orders_parse_input() {
    // Parse inputs
    while (inputs_length() > 0) {
        input_element_t input;
        input_pop(&input);
        orders[input.floor][input.button] = true;
    }
}

/**
 * @brief Get the next floor to move to
 * 
 * @param floor - The newest floor the elevator stopped at
 * @param at_floor - Wheter or not the elevator is at `floor`
 * @param direction - The current prioratized direction of travel
 * @return int representing the next floor to move to
 * @return OR -1 if no floor is found
 */
int orders_get_floor(int floor, int at_floor, int direction) {
    // Always stop on floor if traveling in same direction
    if (at_floor && orders_floor_exists(floor, direction)) {
        return floor;
    }

    LOG_INT(at_floor)

    // Find floors prioratized for upwards travel
    if (direction == DIRN_UP) {
        int found_floor;
        
        found_floor = orders_check_upward(floor + 1);
        if (found_floor != -1) return found_floor;
        
        found_floor = orders_check_downward(N_FLOORS-1);
        if (found_floor != -1) return found_floor;
        
        found_floor = orders_check_upward(0);
        return found_floor;
    }

    // Find floor prioratized for downwards travel
    int found_floor;
    
    found_floor = orders_check_downward(floor - 1);
    if (found_floor != -1) return found_floor;

    found_floor = orders_check_upward(0);
    if (found_floor != -1) return found_floor;

    found_floor = orders_check_downward(N_FLOORS-1);
    return found_floor;
}

int orders_check_upward(int from) {
    for (int floor = from; floor < N_FLOORS; floor++) {
        if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_CAB]) {
            return floor;
        }
    }
    return -1;
}
int orders_check_downward(int from) {
    for (int floor = from; floor >= 0; floor--) {
        if (orders[floor][BUTTON_HALL_DOWN] || orders[floor][BUTTON_CAB]) {
            return floor;
        }
    }
    return -1;
}

void orders_clear_floor(int floor) {
    for (int button = 0; button < N_BUTTONS; button++) {
        orders[floor][button] = false;
    }
}
void orders_clear_all() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        orders_clear_floor(floor);
    }
}

void orders_print() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (orders[floor][button]) {
                const char *description = button == BUTTON_HALL_UP ? "Up" : button == BUTTON_HALL_DOWN ? "Down" : "Cab";
                log_debug("Order at floor %d, button %s", floor, description);
            }
        }
    }
}
