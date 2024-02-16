#include "input.h"

static readonly_elevator_state_t* elevator_state;

static input_linked_list_t* input_linked_list_head = NULL;
static input_linked_list_t* input_linked_list_tail = NULL;

void inputs_read() {
  for(int f = 0; f < N_FLOORS; f++){
    for(int b = 0; b < N_BUTTONS; b++){
      int btnPressed = elevio_callButton(f, b);

      // Check if element already exists in linked list
      if (!input_exists(f, b) && btnPressed) {
        input_push(f, b);
      }
    }
  }
}

void inputs_print(void) {
  // Print
  input_linked_list_t* input_node = input_linked_list_head;

  printf("\nCurrent inputs:\n");

  if (input_node == NULL) {
    printf("[List is empty]\n");
    return;
  }

  while (input_node != NULL) {
    printf("Floor: %d, Button: %d\n", input_node->floor, input_node->button);
    input_node = input_node->next;
  }
}

void input_push(int floor, ButtonType button) {
  // Allocate memory for new input
  input_linked_list_t* new_input = malloc(sizeof(input_linked_list_t));
  elevio_buttonLamp(floor, button, 1);

  // Set values for new input
  new_input->floor = floor;
  new_input->button = button;
  new_input->next = NULL;

  // Update head of linked list
  if (input_linked_list_head == NULL) input_linked_list_head = new_input;
  if (input_linked_list_tail == NULL) input_linked_list_tail = new_input;
  else input_linked_list_tail->next = new_input;
  input_linked_list_tail = new_input;

  // Print linked list
  inputs_print();
}

void input_pop(input_linked_list_t* input_node) {
  if (input_linked_list_head == NULL) return;
  elevio_buttonLamp(input_linked_list_head->floor, input_linked_list_head->button, 0);
  
  // Set input_node to the head of the linked list
  if (input_node != NULL) *input_node = *input_linked_list_head;
  input_linked_list_t* temp = input_linked_list_head;

  // Update tail of linked list
  if (input_linked_list_head == input_linked_list_tail) input_linked_list_tail = NULL;
  input_linked_list_head = input_linked_list_head->next;

  // Delete the head of the linked list node and update the head
  free(temp);

  // Print linked list
  inputs_print();
}

bool input_exists(int floor, int button_type) {
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
