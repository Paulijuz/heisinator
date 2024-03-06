#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "elevio.h"
#include "logger.h"

#include "queue.h"
#include "elevator_fsm.h"

extern bool inputs_registered[N_FLOORS][N_BUTTONS];

typedef struct readonly_elevator_state_s {
    // Passive inputs
    bool obstruction;
    bool stop;
    bool currently_at_floor;

    // States
    int last_known_floor;
} readonly_elevator_state_t;

// Need read-functions (not write)
void inputs_read(void);

bool input_stop_button_pressed();
bool input_stop_button_released();
bool input_stop_button_held();

bool input_door_obstruction();

// Operations
int get_last_floor(void);
int get_current_floor(void);
