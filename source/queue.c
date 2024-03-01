#include "queue.h"

// Calculate direction
int sign(int x) {
  return (x > 0) - (x < 0);
  // return (x != 0) ? x / abs(x) : 0;

}
int dir(int target, int current) {
  int diff = target - current;
  return sign(diff);
}

// Linked list for orders
static order_linked_list_t orders = {
  .head = NULL,
  .tail = NULL,
  .length = 0
};

// Need to differentiate between current direction and operating direction
  // Current direction: The direction the elevator is currently moving
  // Operating direction: The direction of the next order the elevator is going to fulfill

// Maybe we should nest linked lists for "trips" consisting of multiple orders
  // The first order in the trip is always the destination
  // For each new order, if:
    // The order is made to a floor between current and destination
    // AND the order wants to go the same direction as the trip destination

// Do internal events have priority? Should they always be added to the "top" of the queue?

// FSM must handle stop between floors; in case of new order to previous floor, it must move in reverse direction from previously.

void parse_input() {
    input_element_t order;
    if (!input_pop(&order)) return;
    
    int order_direction = (order.button == BUTTON_HALL_UP) - (order.button == BUTTON_HALL_DOWN);

    // If the order queue is empty, add the target floor to the queue
    if (orders_length() == 0) {
        // log_debug("Creating first order");
        order_insert_before(NULL, order.floor, order_direction);
        return;
    }

    // Check if the order already exists
    if (order_exists(order.floor, order_direction)) {
        // log_debug("Order already exists");
        return;
    }

    order_element_t *head = orders.head;
    int previous_floor = get_last_floor();
    int moving_direction = fsm_get_previous_direction();

    // Prioritize current floor
    if (get_current_floor() == order.floor) {
        // log_debug("Order made to current floor");
        order_insert_before(head, order.floor, order_direction);
        return;
    }

    // Start at head
    order_element_t first_order = {
        .floor = previous_floor + moving_direction, // Add direction, so the comparisons are made using the next floor. This prevents an order for the last departed floor from switching the elevator direction.
        .direction = moving_direction,
    };
    order_element_t *destination = orders.head;
    order_element_t *source = &first_order;
    bool order_inserted = false;
    while (destination != NULL && source != NULL) {
        // Calculate limits for checking if the order is made along the current path
        int relative_direction = dir(destination->floor, source->floor);
        int lower_limit = (relative_direction < 0) ? destination->floor : source->floor;
        int upper_limit = (relative_direction < 0) ? source->floor      : destination->floor;

        // Logic for adding order to queue
        if (order_direction == moving_direction || order_direction == 0) {
            if (order.floor >= lower_limit && order.floor <= upper_limit) {
                // log_debug("Order made along current path: %s", order_direction == 0 ? "to let someone off" : "in the same direction");
                order_insert_before(destination, order.floor, order_direction);
                order_inserted = true;
                break;
            }
        } else {
            if (destination->floor * moving_direction > order.floor * moving_direction) {
                // log_debug("Order made in opposite direction");
                order_insert_before(destination, order.floor, order_direction);
                order_inserted = true;
                break;
            }
        }

        // Move to next order in the list
        source = destination;
        destination = destination->next;
    }

    // If the order is not made along the current path, add it to the end of the queue
    if (!order_inserted) {
        // log_debug("Adding order to end of queue:");
        order_insert_before(NULL, order.floor, order_direction);
    }
}

void order_insert_before(order_element_t *reference_node, int floor, int direction) {
    // Allocate memory for new order
    order_element_t* new_order = malloc(sizeof(order_element_t));
    new_order->floor = floor;
    new_order->direction = direction;
    new_order->next = NULL;

    // If the head is null (linked list is empty)
    order_element_t *node = orders.head;
    if (node == NULL) {
        // log_debug("Inserted order at head");
        orders.head = new_order;
        orders.tail = new_order;
    }

    // If the reference_node is NULL, insert at end
    else if (reference_node == NULL) {
        // log_debug("Inserted order at tail");
        if (orders.tail != NULL) orders.tail->next = new_order;
        orders.tail = new_order;
        node = NULL; // Prevents the loop from running
    }

    // Iterate through linked list
    order_element_t *prev_node = orders.head;
    while (node != NULL) {
        // log_debug("Comparing: (%d, %s), (%d, %s)", node->floor, node->direction == 0 ? "s" : node->direction > 0 ? "u" : "d", reference_node->floor, reference_node->direction == 0 ? "s" : reference_node->direction > 0 ? "u" : "d");
        if (order_identical(node, reference_node)) {
            if (node == orders.head) {
                // log_debug("Inserted order before head");
                new_order->next = node;
                orders.head = new_order;
            } else {
                // log_debug("Inserted order before floor: %d", reference_node->floor);
                new_order->next = node;
                prev_node->next = new_order;
            }
        }
        
        prev_node = node;
        node = node->next;
    }

    // Error handling for while loop
    if (node != NULL && node == orders.tail) {
        // Error handling
        log_fatal("Failed to insert order. Reference order not found in queue");
        assert(false);
    }

    // Update length of linked list
    orders.length++;
    orders_print();
    return;
}

bool orders_peek(order_element_t* node) {
    if (orders.head != NULL) {
        *node = *orders.head;
        return true;
    }
    
    return false;
}

bool orders_pop(order_element_t* node) {
    if (orders.head == NULL) return false;

    // Set node to the head of the linked list
    if (node != NULL) *node = *orders.head;
    order_element_t* temp = orders.head;

    // Update tail of linked list
    if (orders.head == orders.tail) orders.tail = NULL;
    orders.head = orders.head->next;

    // Delete the head of the linked list node and update the head
    free(temp);

    // Update length of linked list
    orders.length--;

    return true;
}
bool orders_clear_floor(int floor) {
    if (orders.head == NULL) return false;

    // Keep track of how many orders are removed
    int removed = 0;

    // Start at beginning of linked list
    order_element_t *node = orders.head;
    order_element_t *prev_node = NULL;

    // Iterate through linked list
    while (node != NULL) {
        if (node->floor == floor) {
            order_element_t *temp = node;
            node = node->next;
            
            // Update previous node
            if (prev_node == NULL) {
                orders.head = temp->next;
            } else {
                prev_node->next = temp->next;
            }

            // Update tail of linked list
            if (temp->next == NULL) {
                orders.tail = prev_node;
            }

            free(temp);
            orders.length--;
            removed++;

            continue;
        }

        prev_node = node;
        node = node->next;
    }

    orders_print();
    return (removed > 0);
}

bool orders_clear_all() {
  // Start at beginning of linked list
  order_element_t *node = orders.head;
  
  // Iterate through linked list
  while (node != NULL) {
    order_element_t *temp = node;
    node = node->next;
    free(temp);
  }

  // Update head and tail of linked list
  orders.head = NULL;
  orders.tail = NULL;

  // Update length of linked list
  orders.length = 0;

  return true;
}


bool order_identical(order_element_t* node, order_element_t *reference) {
  return node->floor == reference->floor && node->direction == reference->direction;
}

bool order_exists(int floor, int direction) {
  // Start at beginning of linked list
  order_element_t *node = orders.head;
  
  // Iterate through linked list
  while (node != NULL) {
    if (node->floor == floor && node->direction == direction) {
      return true;
    }
    node = node->next;
  }

  return false;
}

int orders_length() {
  return orders.length;
}

void orders_print() {
  // Start at beginning of linked list
  order_element_t *node = orders.head;
  
  // Iterate through linked list
  log_debug("Orders:");
  while (node != NULL) {
    const char *description = node->direction == 0 ? "Then open" : node->direction > 0 ? "Then up" : "Then down";
    log_debug("Move to %d, (%s)", node->floor, description);
    node = node->next;
  }
  log_debug("");
}
