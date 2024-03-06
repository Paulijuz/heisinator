/**
 * @file input.h
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Header file for input.c
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

/*****************************************************************************/
/* ANSI C includes                                                           */
/*****************************************************************************/
#include <stdbool.h>

/*****************************************************************************/
/* Provided drivers                                                          */
/*****************************************************************************/
#include "elevio.h"

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "logger.h"
#include "queue.h"
#include "elevator_fsm.h"

/**
 * @brief Current state of the elevator
 *
 * @param obstruction True if the door is obstructed
 * @param stop True if the stop button is pressed
 * @param currently_at_floor True if the elevator is currently at a floor
 * @param last_known_floor The last known floor of the elevator
 */
typedef struct readonly_elevator_state_s {
    // Passive inputs
    bool obstruction;
    bool stop;
    bool currently_at_floor;

    // States
    int last_known_floor;
} readonly_elevator_state_t;

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
void inputs_read(void);

bool input_stop_button_pressed();
bool input_stop_button_released();
bool input_stop_button_held();
bool input_door_obstruction();

int get_last_floor(void);
int get_current_floor(void);
