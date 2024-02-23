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

// int main(){
//     elevio_init();
    
//     printf("=== Example Program ===\n");
//     printf("Press the stop button on the elevator panel to exit\n");

//     elevio_motorDirection(DIRN_UP);

//     while(1){
//         int floor = elevio_floorSensor();

//         if(floor == 0){
//             elevio_motorDirection(DIRN_UP);
//         }

//         if(floor == N_FLOORS-1){
//             elevio_motorDirection(DIRN_DOWN);
//         }

//         if(elevio_obstruction()){
//             elevio_stopLamp(1);
//         } else {
//             elevio_stopLamp(0);
//         }
        
//         if(elevio_stopButton()){
//             elevio_motorDirection(DIRN_STOP);
//             break;
//         }
        
//         nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
//     }

//     return 0;
// }

input_element_t current_input;

bool waiting = false;
time_t waiting_until;

// bool moving = false;

int main() {
  // Initialize elevator
  elevio_init();

  // Set light off for all buttons
  for(int f = 0; f < N_FLOORS; f++){
    for(int b = 0; b < N_BUTTONS; b++){
      elevio_buttonLamp(f, b, 0);
    }
  }

  // Print
  printf("=== Program start ===\n");

  // Elevator operation
  while (true) {
    // Read inputs
    inputs_read();

    // Parse inputs
    while (inputs_length() > 0) {
      parse_input(get_last_floor());
    }

    // Execute elevator operation
    elevator_fsm();

    // Check if we are waiting
    // if (waiting) {
    //   if (time(NULL) > waiting_until) {
    //     waiting = false;
    //     printf("\r\033[2KDone waiting\n");
    //   } else {
    //     double remaining_time = waiting_until - time(NULL); // Add one to round up
    //     printf("\rWaiting for %.2f seconds", remaining_time);
    //     fflush(stdout);
    //     continue;
    //     // nanosleep(&(struct timespec){0, 500*1000*1000}, NULL);
    //   }
    // }

    // if(!moving && input_pop(&current_input)) {
    //   // Get input direction with some funky maths (equivalent to sign(dir))
    //   int dir = current_input.floor - last_floor;
    //   elevio_motorDirection(dir != 0 ? abs(dir) / dir : 0);

    //   // Start moving
    //   moving = true;
    //   printf("\nMoving from floor %d to floor %d\n", last_floor, current_input.floor);
    // }

    // if(moving && last_floor == current_input.floor) {
    //   moving = false;
    //   printf("\nArrived at floor %d\n", last_floor);

    //   elevio_motorDirection(DIRN_STOP);

    //   // Start waiting
    //   waiting = true;
    //   waiting_until = time(NULL) + 1;
    //   // waiting_until = clock() + 3*CLOCKS_PER_SEC;
    //   // struct timespec ts = {.tv_sec = 3, .tv_nsec = 0};
    //   // nanosleep(&ts, NULL);

    //   current_input.floor = -1;
    //   current_input.button = -1;
    // }

    if(elevio_stopButton()) {
      log_info("Queue deleted.");
      input_pop(NULL);
    }
  }
}
