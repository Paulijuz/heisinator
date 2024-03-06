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
#include "elevio.h"

/*****************************************************************************/
/* Module includes                                                           */
/*****************************************************************************/
#include "input.h"

/*****************************************************************************/
/* Public API                                                                */
/*****************************************************************************/
bool queue_add_order(int floor, ButtonType button);
bool queue_get_order(int floor, ButtonType button);

bool queue_order_exists(int floor, int direction);
bool queue_any_orders();

int  queue_get_direction(int floor, int at_floor, int direction);

void queue_clear_floor(int floor);
void queue_clear_all();

void queue_print();
