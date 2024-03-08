/**
 * @file input.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief File for reading inputs from the elevator
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "input.h"

/*****************************************************************************/
/* Local variables                                                           */
/*****************************************************************************/
static bool inputs_held[N_FLOORS][N_BUTTONS] = {0};

static bool stop_button_state_prev = false;
static bool stop_button_state_curr = false;

static bool door_obstruction = false;

static int current_floor = -1;
static int last_floor = -1;

/*****************************************************************************/
/* Function definitions                                                      */
/*****************************************************************************/
/**
 * @brief Read inputs from the elevator
 */
void inputs_read() {
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            // Check if button is held
            int button_pressed = elevio_callButton(f, b);
            bool button_held = inputs_held[f][b];

            // Check if element already exists in linked list
            if (button_pressed && !button_held) {
                orders_add(f, b);
            }

            // Update inputs_held
            inputs_held[f][b] = button_pressed;
        }
    }

    current_floor = elevio_floorSensor();
    if (current_floor != -1 && last_floor != current_floor) {
        log_info("Current floor: %d", current_floor);
        last_floor = current_floor;
    }

    stop_button_state_prev = stop_button_state_curr;
    stop_button_state_curr = elevio_stopButton();

    door_obstruction = elevio_obstruction();
}

/**
 * @brief Check if stop button is pressed
 *
 * @return bool
 */
bool input_stop_button_pressed() {
  return stop_button_state_curr && !stop_button_state_prev;
}

/**
 * @brief Check if stop button is released
 *
 * @return bool
 */
bool input_stop_button_released() {
  return !stop_button_state_curr && stop_button_state_prev;
}

/**
 * @brief Check if stop button is held
 *
 * @return bool
 */
bool input_stop_button_held() {
    return stop_button_state_curr;
}

/**
 * @brief Check if door is obstructed
 *
 * @return bool
 */
bool input_door_obstruction() {
    return door_obstruction;
}

/**
 * @brief Get the last floor the elevator was at
 *
 * @return Last floor
 */
int get_last_floor(void) {
    return last_floor;
}

/**
 * @brief Get the current floor the elevator is at
 *
 * @return int representing the current floor, or -1 if the elevator is between floors
 */
int get_current_floor(void) {
    return current_floor;
}
