#include "input.h"

static readonly_elevator_state_t* elevator_state;

static input_linked_list_t* input_linked_list_head = NULL;
static input_linked_list_t* input_linked_list_tail = NULL;

void read_inputs() {
  for(int f = 0; f < N_FLOORS; f++){
    for(int b = 0; b < N_BUTTONS; b++){
      int btnPressed = elevio_callButton(f, b);
      printf("Button pressed: %d\n", btnPressed);
      elevio_buttonLamp(f, b, btnPressed);

      // Check if element already exists in linked list
      if (!exists_in_input(f, b) && btnPressed) {
        push_input(f, b);
      }
    }
  }

  // Print
  input_linked_list_t* input_node = input_linked_list_head;
  while (input_node != NULL) {
    printf("Floor: %d, Button: %d\n", input_node->floor, input_node->button);
    input_node = input_node->next;
  }
  printf("\n");
}


void push_input(int floor, ButtonType button) {
  // Allocate memory for new input
  input_linked_list_t* new_input = malloc(sizeof(input_linked_list_t));

  // Set values for new input
  new_input->floor = floor;
  new_input->button = button;
  new_input->next = NULL;

  // Update head of linked list
  if (input_linked_list_head == NULL) input_linked_list_head = new_input;
  if (input_linked_list_tail == NULL) input_linked_list_tail = new_input;
  else input_linked_list_tail->next = new_input;
}

void pop_input(input_linked_list_t* input_node) {
  if (input_linked_list_head == NULL);
  
  // Set input_node to the head of the linked list
  *input_node = *input_linked_list_head;

  // Delete the head of the linked list node and update the head
  input_linked_list_t* temp = input_linked_list_head;
  input_linked_list_head = input_linked_list_head->next;
  free(temp);
}

bool exists_in_input(int floor, int button_type) {
  // Start at beginning of linked list
  input_linked_list_t *input_node = input_linked_list_head;
  
  // Iterate through linked list
  while (input_node != NULL) {
    if (input_node->floor == floor && input_node->button == button_type) {
      return true;
    }
    
    input_node = input_node->next;
  }

  return false;
}
