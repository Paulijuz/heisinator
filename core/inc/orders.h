/**
 * @file orders.h
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Header file for orders.c
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
#include <stdbool.h>

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
void orders_add(int floor, ButtonType button);
bool orders_get(int floor, ButtonType button);

bool orders_direction_exists(int floor, int direction);
bool orders_any();

int  orders_determine_next(int floor, bool at_floor, int direction);

void orders_clear_floor(int floor);
void orders_clear_all();

void orders_print();
