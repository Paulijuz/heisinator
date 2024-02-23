#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "driver/elevio.h"

#include "input.h"

extern bool operating;
extern int destination;

// Linked list for orders
typedef struct order_element_s {
  int floor;
  int direction; // When we get to the floor, which direction should we go?
  
  struct order_element_s *next;
} order_element_t;

typedef struct order_linked_list_s {
  order_element_t *head;
  order_element_t *tail;
  int length;
} order_linked_list_t;

// Logic function
void parse_input(int current_floor);

// Linked list helper functions
void order_insert_before(order_element_t*, int floor, int direction);
bool order_pop(order_element_t* node);

bool order_identical(order_element_t* node, order_element_t *reference);
bool order_exists(int floor, int direction);
int orders_length();
void orders_print();
