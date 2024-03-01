#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "driver/elevio.h"

#include "input.h"

// API
bool orders_exists();
int orders_get_floor(int floor, int direction);
void orders_clear_floor(int floor);
void orders_clear_all();
void orders_print();
