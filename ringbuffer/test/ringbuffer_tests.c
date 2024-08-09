#include "minunit.h"
#include "../ringbuffer.h"

char *test_RingBuffer_create() {

    return NULL;
}

char *test_RingBuffer_destroy() {

    return NULL;
}

char *test_RingBuffer_write() {

    return NULL;
}

char *test_RingBuffer_read() {

    return NULL;
}

char *test_RingBuffer_gets() {

    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_RingBuffer_create);
    mu_run_test(test_RingBuffer_write);
    mu_run_test(test_RingBuffer_read);
    mu_run_test(test_RingBuffer_gets);
    mu_run_test(test_RingBuffer_destroy);

    return NULL;
}

RUN_TESTS(all_tests);
