#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "driver/elevio.h"

#include "input.h"

extern bool orders[N_FLOORS][N_BUTTONS];

// API
bool orders_floor_exists(int floor);
bool orders_any_exist();
int orders_get_floor(int floor, bool on_floor, int direction);

void orders_parse_input();

void orders_clear_floor(int floor);
void orders_clear_all();

void orders_print();
