#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <math.h>
#include "driver/elevio.h"

#include "logger.h"
#include "input.h"


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

input_linked_list_t current_input;

int main() {
  elevio_init();

  // Set light off for all buttons
  for(int f = 0; f < N_FLOORS; f++){
    for(int b = 0; b < N_BUTTONS; b++){
      elevio_buttonLamp(f, b, 0);
    }
  }

  // Startup
  elevio_motorDirection(DIRN_DOWN);
  while(last_floor == -1) inputs_read();
  elevio_motorDirection(DIRN_STOP);

  bool moving = false;

  while (true) {
    inputs_read();

    if(!moving && input_pop(&current_input)) {
      int dir = current_input.floor - last_floor;
      elevio_motorDirection(dir != 0 ? abs(dir) / dir : 0);
      
    //   if(dir > 0) {
    //     elevio_motorDirection(DIRN_UP);
    //   } else if(dir < 0) {
    //     elevio_motorDirection(DIRN_DOWN);
    //   }

      moving = true;
      log_info("Moving %s from floor %d to floor %d", dir > 0 ? "up" : "down", last_floor, current_input.floor);
    }

    if(moving && last_floor == current_input.floor) {
      moving = false;

      log_info("Arrived at floor %d", last_floor);
      log_warning("OH NO!");
      log_error("wow");
      elevio_motorDirection(DIRN_STOP);
      struct timespec ts;
      ts.tv_sec = 3;
      ts.tv_nsec = 0;

      nanosleep(&ts, NULL);

      current_input.floor = -1;
      current_input.button = -1;
    }
    
    if(elevio_stopButton()) {
      log_fatal("DELETE!");
      input_pop(NULL);
    }
  }
}
