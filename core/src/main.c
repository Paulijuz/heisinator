/**
 * @file main.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Main file for the elevator project
 * @version 0.1
 * @date 2024-03-06
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* All file includes                                                         */
/*****************************************************************************/
#include "elevio.h"
#include "input.h"
#include "fsm.h"
#include "light_sync.h"

int main() {
  // Initialize elevator
  elevio_init();

  // Set light off for all buttons
  light_sync_init();

  // Elevator operation
  while (true) {
    // Read inputs
    inputs_read();

    // Execute elevator operation
    fsm();

    // Update panel lights
    light_sync();
  }
}
