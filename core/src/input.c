#include "input.h"

static bool inputs_held[N_FLOORS][N_BUTTONS] = {0};

static bool stop_button_state_prev = false;
static bool stop_button_state_curr = false;

static bool door_obstruction = false;

static int current_floor = -1;
static int last_floor = -1;

void inputs_read() {
    for(int f = 0; f < N_FLOORS; f++){
        for(int b = 0; b < N_BUTTONS; b++){
            // Check if button is held
            int button_pressed = elevio_callButton(f, b);
            bool button_held = inputs_held[f][b];

            // Check if element already exists in linked list
            if (button_pressed && !button_held) {
                queue_add_order(f, b);
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

bool input_stop_button_pressed() {
  return stop_button_state_curr && !stop_button_state_prev;
}
bool input_stop_button_released() {
  return !stop_button_state_curr && stop_button_state_prev;
}
bool input_stop_button_held() {
    return stop_button_state_curr;
}

bool input_door_obstruction() {
    return door_obstruction;
}

// Set last floor
int get_last_floor(void) {
    return last_floor;
}

// Get current floor
int get_current_floor(void) {
    return current_floor;
}
