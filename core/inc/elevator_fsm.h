/**
 * @file elevator_fsm.h
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Header file for elevator_fsm.c
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

/**
 * @brief Enum for the different states the elevator can be in
 *
 * @param STARTUP Initial state of the elevator
 * @param STOPPED Elevator isn't moving
 * @param MOVING Elevator is moving
 * @param EMERGENCY_STOP Immediate stop when emergency button is pressed
 */
typedef enum states_e {
    STARTUP,
    STOPPED,
    MOVING,
    EMERGENCY_STOP
} states_t;

/**
 * @brief Enum for the different states the door can be in
 *
 * @param DOOR_OPEN Door is open
 * @param DOOR_CLOSED Door is closed
 */
typedef enum door_state_e {
    DOOR_OPEN,
    DOOR_CLOSED
} door_state_t;

/**
 * @brief Struct for the different states and their operations
 *
 * @param state Unique identifier for the state
 * @param name The name of the state
 * @param operation The operation to be performed when in the state
 */
typedef struct state_config_s {
    states_t state;
    char    *name;
    void (*operation)(void);
} state_config_t;

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
void     elevator_fsm(void);
states_t get_state(void);
void     set_state(states_t state);

bool     get_door_open();
