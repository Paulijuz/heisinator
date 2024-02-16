#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "driver/elevio.h"

// struct {
//   // Active inputs
//   linked_list *active_inputs;
// }

// Linked list for inputs
typedef struct input_linked_list_s {
  int floor;
  ButtonType button;
  struct input_linked_list_s *next;
} input_linked_list_t;


typedef struct readonly_elevator_state_s {
  // Passive inputs
  bool obstruction;
  bool stop;
  bool currently_at_floor;

  // States
  int last_known_floor;
} readonly_elevator_state_t;

// Need read-functions (not write)
void read_inputs(void);

// 

void push_input(int floor, ButtonType button);
void pop_input(input_linked_list_t* input_node);
bool exists_in_input(int floor, int button_type);
