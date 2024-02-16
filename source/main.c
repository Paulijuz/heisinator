#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"

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

int main() {
  elevio_init();

  printf("=== Button Test Program ===\n");
  printf("Press the stop button on the elevator panel to exit\n");  

  while (true) {
    read_inputs();

    if(elevio_stopButton()) {
      pop_input(NULL);
      break;
    }
  }
}
