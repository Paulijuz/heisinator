/**
 * @file queue.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Implementation of the queue module
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 */

/*****************************************************************************/
/* ANSI C libraries                                                          */
/*****************************************************************************/
#include <assert.h>

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "queue.h"

// Macros
/**
 * @brief Convert a direction to a button
 * @details  0 -> BUTTON_CAB
 *           1 -> BUTTON_HALL_UP
 *          -1 -> BUTTON_HALL_DOWN
 */
#define DIR_TO_BTN(x) x == 0 ? BUTTON_CAB : (x > 0 ? BUTTON_HALL_UP : BUTTON_HALL_DOWN)

// Local declarations
static int queue_check_upward(int floor);
static int queue_check_downward(int floor);

// Array of orders
bool orders[N_FLOORS][N_BUTTONS] = {0};

/**
 * Checks if a given order exists.
 *
 * @param floor - Floor of order (0-indexed)
 * @param direction - Direction of order (-1, 0, or 1)
 */
bool queue_order_exists(int floor, int direction) {
    for (int button = 0; button < N_BUTTONS; button++) {
        if (orders[floor][DIR_TO_BTN(direction)] || orders[floor][BUTTON_CAB]) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if any orders exist
 *
 * @return true if any orders exist
 * @return false if no orders exist
 */
bool queue_any_orders() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (orders[floor][button]) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Add an order to the queue
 */
bool queue_add_order(int floor, ButtonType button) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    orders[floor][button] = true;
    return true;
}

/**
 * @brief Get an order from the queue
 */
bool queue_get_order(int floor, ButtonType button) {
    assert(floor >= 0);
    assert(floor < N_FLOORS);

    return orders[floor][button];
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
int queue_get_direction(int floor, int at_floor, int direction) {
    // Always stop on floor if traveling in same direction
    if (at_floor && queue_order_exists(floor, direction)) {
        return floor;
    }

    LOG_INT(at_floor)

    // Find floors prioratized for upwards travel
    if (direction == DIRN_UP) {
        int found_floor;

        found_floor = queue_check_upward(floor + 1);
        if (found_floor != -1)
            return found_floor;

        found_floor = queue_check_downward(N_FLOORS - 1);
        if (found_floor != -1)
            return found_floor;

        found_floor = queue_check_upward(0);
        return found_floor;
    }

    // Find floor prioratized for downwards travel
    int found_floor;

    found_floor = queue_check_downward(floor - 1);
    if (found_floor != -1)
        return found_floor;

    found_floor = queue_check_upward(0);
    if (found_floor != -1)
        return found_floor;

    found_floor = queue_check_downward(N_FLOORS - 1);
    return found_floor;
}

/**
 * @brief Check for orders upwards from a given floor
 *
 * @param from Floor to start checking from
 * @return int representing the next floor to move to, or -1 if no floor is found
 */
int queue_check_upward(int from) {
    for (int floor = from; floor < N_FLOORS; floor++) {
        if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_CAB]) {
            return floor;
        }
    }
    return -1;
}

/**
 * @brief Check for orders downwards from a given floor
 *
 * @param from The floor to check orders from.
 * @return int representing the next floor to move to, or -1 if no floor is found
 */
int queue_check_downward(int from) {
    for (int floor = from; floor >= 0; floor--) {
        if (orders[floor][BUTTON_HALL_DOWN] || orders[floor][BUTTON_CAB]) {
            return floor;
        }
    }
    return -1;
}

/**
 * @brief Clear all orders at a given floor
 *
 * @param floor Floor to clear orders from
 */
void queue_clear_floor(int floor) {
    for (int button = 0; button < N_BUTTONS; button++) {
        orders[floor][button] = false;
    }
}

/**
 * @brief Clear all orders
 */
void queue_clear_all() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        queue_clear_floor(floor);
    }
}

/**
 * @brief Print all orders
 */
void queue_print() {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            if (orders[floor][button]) {
                const char *description = button == BUTTON_HALL_UP ? "Up" : button == BUTTON_HALL_DOWN ? "Down"
                                                                                                       : "Cab";
                log_debug("Order at floor %d, button %s", floor, description);
            }
        }
    }
}
