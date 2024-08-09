#include "minunit.h"
#include "../src/ringbuffer/ringbuffer.h"

RingBuffer *buffer;
const int BUFFER_LEN = 100;
char *TEST_DATA = "abcdefghijklmnopqrstuvwxyz";
const int TEST_DATA_LEN = 26;

char *test_RingBuffer_create() {
    buffer = RingBuffer_create(BUFFER_LEN);
    mu_assert(buffer != NULL, "Failed to create ring buffer.");
    mu_assert(buffer->buffer != NULL, "Failed to create buffer->buffer.");
    mu_assert(buffer->length == BUFFER_LEN + 1,
              "Wrong buffer length.");
    mu_assert(buffer->start == 0, "Wrong buffer start.");
    mu_assert(buffer->end == 0, "Wrong buffer end.");

    RingBuffer_destroy(&buffer);

    return NULL;
}

char *test_RingBuffer_destroy() {
    buffer = RingBuffer_create(BUFFER_LEN);
    RingBuffer_destroy(&buffer);
    mu_assert(buffer == NULL, "buffer should be NULL.");

    return NULL;
}

char *test_RingBuffer_write() {
    buffer = RingBuffer_create(BUFFER_LEN);
    int rc = RingBuffer_write(buffer, TEST_DATA, TEST_DATA_LEN);
    mu_assert(rc == TEST_DATA_LEN, "Wrong amount returned.");
    mu_assert(RingBuffer_ends_at(buffer) == RingBuffer_starts_at(buffer)
              + TEST_DATA_LEN, "Wrong buffer end.");

    RingBuffer_destroy(&buffer);

    return NULL;
}

char *test_RingBuffer_read() {
    buffer = RingBuffer_create(BUFFER_LEN);
    RingBuffer_write(buffer, TEST_DATA, TEST_DATA_LEN);
    char result[TEST_DATA_LEN];
    int amount = RingBuffer_read(buffer, result, TEST_DATA_LEN);
    mu_assert(amount == TEST_DATA_LEN, "Wrong amount returned.");
    mu_assert(RingBuffer_starts_at(buffer) == RingBuffer_ends_at(buffer),
              "Wrong buffer start.");

    RingBuffer_destroy(&buffer);

    return NULL;
}

char *test_RingBuffer_gets() {
    buffer = RingBuffer_create(BUFFER_LEN);
    RingBuffer_write(buffer, TEST_DATA, TEST_DATA_LEN);
    bstring result = RingBuffer_gets(buffer, 3);
    bstring expected_result = cstr2bstr("abc");
    mu_assert(bstrcmp(result, expected_result) == 0, "Wrong return buffer.");

    bdestroy(result);
    bdestroy(expected_result);
    RingBuffer_destroy(&buffer);

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
