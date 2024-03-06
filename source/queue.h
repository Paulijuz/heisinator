/**
 * @file queue.h
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Header file for queue.c
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

/*****************************************************************************/
/* ANSI C libraries                                                          */
/*****************************************************************************/
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*****************************************************************************/
/* Provided drivers                                                          */
/*****************************************************************************/
#include "driver/elevio.h"

/*****************************************************************************/
/* Custom header files                                                       */
/*****************************************************************************/
#include "input.h"

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
/**
 * @brief Array of orders
 */
extern bool orders[N_FLOORS][N_BUTTONS];

// Functions
void orders_parse_input();

bool orders_floor_exists(int floor, int direction);
bool orders_any_exist();

int  orders_get_floor(int floor, int at_floor, int direction);

void orders_clear_floor(int floor);
void orders_clear_all();

void orders_print();
