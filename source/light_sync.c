#include "light_sync.h"

static bool buttom_lamp_states[N_FLOORS][N_BUTTONS] = {false};
static bool stop_button_lamp_state = false;
static bool door_lamp_state = false;
static int floor_indicator_state = 0;

/**
 * Turns of all lights
*/
void light_sync_init() {
    log_info("Initializing lamps");

    for(int floor = 0; floor < N_FLOORS; floor++) {
        for(int button_type = 0; button_type < N_BUTTONS; button_type++) {
            elevio_buttonLamp(floor, button_type, 0);
        }
    }

    elevio_stopLamp(0);

    elevio_floorIndicator(0);
}

/**
 * Sets lights to correspond with order queue, stop button state and last floor.
*/
void light_sync() {
    // Button lamps
    for(int floor = 0; floor < N_FLOORS; floor++) {
        for(int button_type = 0; button_type < N_BUTTONS; button_type++) {
            bool order_exists = orders[floor][button_type];
            if (buttom_lamp_states[floor][button_type] != order_exists) {
                buttom_lamp_states[floor][button_type] = true;
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
    if (floor_indicator_state != last_floor && last_floor != -1) {
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
