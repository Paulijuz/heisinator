#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"

#include "queue.h"

// struct {
//   // Active inputs
//   linked_list *active_inputs;
// }

extern int last_floor;

typedef struct input_element_s {
  int floor;
  ButtonType button;
  struct input_element_s *next;
} input_element_t;

typedef struct input_linked_list_s {
  input_element_t *head;
  input_element_t *tail;
  int length;
} input_linked_list_t;
extern input_linked_list_t* input_linked_list_head;

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

// Linked list helper functions
void input_push(int floor, ButtonType button);
bool input_pop(input_element_t* input_node);
bool input_exists(int floor, int button_type);
int input_length(void);
void inputs_print();
