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
void inputs_read(void);

// 

void input_push(int floor, ButtonType button);
void input_pop(input_linked_list_t* input_node);
bool input_exists(int floor, int button_type);
void inputs_print(void);
