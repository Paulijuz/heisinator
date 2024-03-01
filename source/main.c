#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include "driver/elevio.h"

#include "logger.h"
#include "input.h"
#include "queue.h"
#include "elevator_fsm.h"
#include "light_sync.h"

int main() {
    // Initialize elevator
    elevio_init();

    // Print
    printf("=== Program start ===\n");

    // Set light off for all buttons
    light_sync_init();

    // Elevator operation
    while (true) {
        // Read inputs
        inputs_read();

        // Parse inputs
        orders_parse_input();

        // Execute elevator operation
        elevator_fsm();

        light_sync();
    }
}
