/**
 * @file orders_test.c
 * @author Theodor Johansson (theodor.lund.johansson@gmail.com)
 * @brief Test file for the orders module
 * @version 0.1
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024
 *
 */

/*****************************************************************************/
/* Module libraries                                                          */
/*****************************************************************************/
#include "orders.h"
#include "unity.h"

/*****************************************************************************/
/* Set up/tear down                                                          */
/*****************************************************************************/
void orders_tests_set_up(void) {
    // This is run before EACH test
}
void orders_tests_tear_down(void) {
    orders_clear_all();
}

/*****************************************************************************/
/* Unit tests                                                                */
/*****************************************************************************/
void test_add_and_get(void) {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            TEST_ASSERT_FALSE(orders_get(floor, button));
            orders_add(floor, button);
            TEST_ASSERT_TRUE(orders_get(floor, button));
        }
    }
}
void test_clear_all(void) {
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            orders_add(floor, button);
        }
    }
    orders_clear_all();
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int button = 0; button < N_BUTTONS; button++) {
            TEST_ASSERT_FALSE(orders_get(floor, button));
        }
    }
}

void orders_tests_run(void) {
    RUN_TEST(test_add_and_get);
    RUN_TEST(test_clear_all);
}
