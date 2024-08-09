#include "minunit.h"
#include "../src/ringbuffer/ringbuffer.h"

RingBuffer *buffer;

char *test_RingBuffer_create() {
    buffer = RingBuffer_create(10);
    mu_assert(buffer != NULL, "Failed to create ring buffer.");

    return NULL;
}

char *test_RingBuffer_destroy() {
    RingBuffer_destroy(buffer);

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
