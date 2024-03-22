#include "unity.h"
#include "orders_tests.h"

typedef struct test_suite_s {
    char name[16];
    void (*set_up)(void);
    void (*tear_down)(void);
    void (*run)(void);
} test_suite_t;

test_suite_t test_suites[] = {
    { "Orders", orders_tests_set_up, orders_tests_tear_down, orders_tests_run },
};

const int test_suite_len = sizeof(test_suites)/sizeof(test_suites[0]);

test_suite_t current_test_suite;

void setUp(void) {
    current_test_suite.set_up();
}

void tearDown(void) {
    current_test_suite.tear_down();
}

int main() {
    UNITY_BEGIN();
    
    for(int i = 0; i<test_suite_len; i++) {
        current_test_suite = test_suites[i];

        printf("Running tests for '%s'.\n", current_test_suite.name);
        current_test_suite.run();
    }

    return UNITY_END();
}