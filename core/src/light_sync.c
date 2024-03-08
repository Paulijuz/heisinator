/**
 * @file light_sync.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Module for synchronizing panel lights with the corresponding internal active inputs.
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* ANSI C libraries                                                          */
/*****************************************************************************/
#include <stdbool.h>

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "queue.h"
#include "light_sync.h"

/*****************************************************************************/
/* Defines                                                                   */
/*****************************************************************************/
#define FLOOR_INDICATOR_ROULETTE true

/*****************************************************************************/
/* Local variables                                                           */
/*****************************************************************************/
/**
 * @brief Array for storing the state of the panel lights.
 * @note By remembering the state of the panel lights, we can avoid unnecessary calls to the elevio API, which is slow as fuck for some reason.
 */
static bool buttom_lamp_states[N_FLOORS][N_BUTTONS] = {false};

static bool stop_button_lamp_state = false; // Wether the stop button lamp should be lit
static bool door_lamp_state        = false; // Wether the door lamp should be lit
static int  floor_indicator_state  = 0;     // Which floor indicator lamp should be lit

/**
 * @brief Turns off all panel lights.
 */
void light_sync_init() {
    log_info("Initializing lamps");

    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button_type = 0; button_type < N_BUTTONS; button_type++) {
            elevio_buttonLamp(floor, button_type, 0);
        }
    }

    elevio_stopLamp(0);

    elevio_floorIndicator(0);
}

/**
 * @brief Sets lights to correspond with order queue, stop button state and last floor.
 */
void light_sync() {
    // Button lamps
    for(int floor = 0; floor < N_FLOORS; floor++) {
        for(int button_type = 0; button_type < N_BUTTONS; button_type++) {
            bool order_exists = queue_get_order(floor, button_type);
            if (buttom_lamp_states[floor][button_type] != order_exists) {
                buttom_lamp_states[floor][button_type] = order_exists;
                elevio_buttonLamp(floor, button_type, order_exists);
            }
        }
    }

    // Stop button lamp
    bool stop_button_state = input_stop_button_held();
    if (stop_button_lamp_state != stop_button_state) {
        stop_button_lamp_state = stop_button_state;
        elevio_stopLamp(stop_button_lamp_state);
    }

    // Floor indicator lamps
    int last_floor = get_last_floor();
    if (last_floor == -1) {
        if (FLOOR_INDICATOR_ROULETTE) {
            static int i = 0;
            elevio_floorIndicator((i++ / 16) % N_FLOORS);
        }
    } else if (floor_indicator_state != last_floor) {
        floor_indicator_state = last_floor;
        elevio_floorIndicator(floor_indicator_state);
    }

    // Door lamp
    bool door_open = get_door_open();
    if (door_lamp_state != door_open) {
        door_lamp_state = door_open;
        elevio_doorOpenLamp(door_lamp_state);
    }
}
