#include "input.h"

// static readonly_elevator_state_t* elevator_state;

// Linked list for inputs
static input_linked_list_t input_linked_list = {
  .head = NULL,
  .tail = NULL,
  .length = 0
};

int last_floor = -1;
bool inputs_held[N_FLOORS][N_BUTTONS] = {0};

void inputs_read() {
  for(int f = 0; f < N_FLOORS; f++){
    for(int b = 0; b < N_BUTTONS; b++){
      // Check if button is held
      int button_pressed = elevio_callButton(f, b);
      bool button_held = inputs_held[f][b];

      // Check if element already exists in linked list
      if (button_pressed && !button_held && !input_exists(f, b)) {
        input_push(f, b);
      }

      // Update inputs_held
      inputs_held[f][b] = button_pressed;
    }
  }

  int floor = elevio_floorSensor();
  if (floor != -1) last_floor = floor;
}

void inputs_print() {
  // Print
  // printf("\033[2J"); // Clear screen
  printf("\nCurrent inputs:\n");

  input_element_t* input_node = input_linked_list.head;
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
  input_element_t* new_input = malloc(sizeof(input_element_t));
  elevio_buttonLamp(floor, button, 1);

  // Set values for new input
  new_input->floor = floor;
  new_input->button = button;
  new_input->next = NULL;
  
  // Update head and tail of linked list
  if (input_linked_list.head == NULL) {
    input_linked_list.head = new_input;
    input_linked_list.tail = new_input;
  } else {
    input_linked_list.tail->next = new_input;
    input_linked_list.tail = new_input;
  }

  // Update length of linked list
  input_linked_list.length++;

  // Print linked list
  // inputs_print();
}

bool input_pop(input_element_t* input_node) {
  if (input_linked_list.head == NULL) return false;
  elevio_buttonLamp(input_linked_list.head->floor, input_linked_list.head->button, 0);
  
  // Set input_node to the head of the linked list
  if (input_node != NULL) *input_node = *input_linked_list.head;
  input_element_t* temp = input_linked_list.head;

  // Update tail of linked list
  if (input_linked_list.head == input_linked_list.tail) input_linked_list.tail = NULL;
  input_linked_list.head = input_linked_list.head->next;

  // Delete the head of the linked list node and update the head
  free(temp);

  // Print linked list
  // inputs_print();

  // Update length of linked list
  input_linked_list.length--;

  return true;
}

int input_length(void) {
  return input_linked_list.length;
}

bool input_exists(int floor, int button_type) {
  // Start at beginning of linked list
  input_element_t *input_node = input_linked_list.head;
  
  // Iterate through linked list
  while (input_node != NULL) {
    if (input_node->floor == floor && input_node->button == button_type) {
      return true;
    }
    
    input_node = input_node->next;
  }

  return false;
}
