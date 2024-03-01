#include "queue.h"

int orders_check_upward(int floor);
int orders_check_downward(int floor);

// Array of orders
bool orders[N_FLOORS][N_BUTTONS] = {0};

bool orders_exists() {
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
 * @param floor 
 * @param direction 
 * @return int representing the next floor to move to
 * @return OR -1 if no floor is found
 */
int orders_get_floor(int floor, int direction) {
    // Upward first
    if (direction == DIRN_UP || direction == DIRN_STOP) {
        int upward = orders_check_upward(floor);
        if (upward != -1) return upward;
        int downwards = orders_check_downward(floor);
        return downwards;
    }

    // Downward first
    else {
        int downwards = orders_check_downward(floor);
        if (downwards != -1) return downwards;
        int upward = orders_check_upward(floor);
        return upward;
    }
}
int orders_check_upward(int floor) {
    for (int i = floor; i < N_FLOORS; i++) {
        if (orders[i][BUTTON_HALL_UP] || orders[i][BUTTON_CAB]) {
            return i;
        }
    }
    return -1;
}
int orders_check_downward(int floor) {
    for (int i = floor; i >= 0; i--) {
        if (orders[i][BUTTON_HALL_DOWN] || orders[i][BUTTON_CAB]) {
            return i;
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
