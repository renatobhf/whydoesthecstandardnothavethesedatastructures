#include <stdlib.h>
#include <string.h>

#include "unity.h"

/* include your dynamic array header (assumes it is in the project root) */
#include "dynamic_array.h"

DYN_ARRAY_DECLARE(dyn_array_int, int);
DYN_ARRAY_IMPLEMENT(dyn_array_int, int);

/* Helper to create a dyn array with a small capacity to force growth */
static dyn_array_int_t *make_small_array(void) { return dyn_array_int_create(2); }

void setUp(void) {}
void tearDown(void) {}

/* Helper macro to compare size_t values using Unity */
#define TEST_ASSERT_EQUAL_SIZE_T(expected, actual) \
    TEST_ASSERT_EQUAL_UINT((unsigned int)(expected), (unsigned int)(actual))

void test_init_and_properties(void) {
    dyn_array_int_t *a = make_small_array();
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_NOT_NULL(a->data);
    TEST_ASSERT_EQUAL_SIZE_T(0, dyn_array_int_size(a));
    TEST_ASSERT_EQUAL_SIZE_T(2, dyn_array_int_capacity(a));
    dyn_array_int_free(a);
}

void test_push_and_get_and_size(void) {
    dyn_array_int_t *a = make_small_array();
    TEST_ASSERT_TRUE(dyn_array_int_push_back(a, 10));
    TEST_ASSERT_TRUE(dyn_array_int_push_back(a, 20));
    TEST_ASSERT_EQUAL_SIZE_T(2, dyn_array_int_size(a));
    TEST_ASSERT_EQUAL_INT(10, dyn_array_int_get(a, 0));
    TEST_ASSERT_EQUAL_INT(20, dyn_array_int_get(a, 1));

    /* pushing one more should trigger growth (capacity from 2 -> 4) */
    TEST_ASSERT_TRUE(dyn_array_int_push_back(a, 30));
    TEST_ASSERT_EQUAL_SIZE_T(3, dyn_array_int_size(a));
    TEST_ASSERT_TRUE(dyn_array_int_capacity(a) >= 3);
    TEST_ASSERT_EQUAL_INT(30, dyn_array_int_get(a, 2));

    dyn_array_int_free(a);
}

void test_pop_back_and_empty_pop(void) {
    dyn_array_int_t *a = make_small_array();
    dyn_array_int_push_back(a, 1);
    dyn_array_int_push_back(a, 2);
    TEST_ASSERT_EQUAL_INT(2, dyn_array_int_pop_back(a));
    TEST_ASSERT_EQUAL_SIZE_T(1, dyn_array_int_size(a));
    TEST_ASSERT_EQUAL_INT(1, dyn_array_int_pop_back(a));
    TEST_ASSERT_EQUAL_SIZE_T(0, dyn_array_int_size(a));

    /* pop from empty returns EMPTY_TYPE (0) */
    TEST_ASSERT_EQUAL_INT(0, dyn_array_int_pop_back(a));

    dyn_array_int_free(a);
}

void test_get_oob_returns_empty_type(void) {
    dyn_array_int_t *a = make_small_array();
    /* empty array: any get returns EMPTY_TYPE (0) */
    TEST_ASSERT_EQUAL_INT(0, dyn_array_int_get(a, 0));
    dyn_array_int_free(a);
}

void test_set_and_set_oob(void) {
    dyn_array_int_t *a = make_small_array();
    dyn_array_int_push_back(a, 5);
    TEST_ASSERT_TRUE(dyn_array_int_set(a, 0, 42));
    TEST_ASSERT_EQUAL_INT(42, dyn_array_int_get(a, 0));
    /* set OOB must fail */
    TEST_ASSERT_FALSE(dyn_array_int_set(a, 5, 7));
    dyn_array_int_free(a);
}

void test_insert_middle_and_front(void) {
    dyn_array_int_t *a = make_small_array();
    dyn_array_int_push_back(a, 1);
    dyn_array_int_push_back(a, 3);
    /* insert 2 at index 1 -> [1,2,3] */
    TEST_ASSERT_TRUE(dyn_array_int_insert(a, 1, 2));
    TEST_ASSERT_EQUAL_SIZE_T(3, dyn_array_int_size(a));
    TEST_ASSERT_EQUAL_INT(1, dyn_array_int_get(a, 0));
    TEST_ASSERT_EQUAL_INT(2, dyn_array_int_get(a, 1));
    TEST_ASSERT_EQUAL_INT(3, dyn_array_int_get(a, 2));

    /* insert at front index 0 */
    TEST_ASSERT_TRUE(dyn_array_int_insert(a, 0, 99));
    TEST_ASSERT_EQUAL_INT(99, dyn_array_int_get(a, 0));
    TEST_ASSERT_EQUAL_SIZE_T(4, dyn_array_int_size(a));

    dyn_array_int_free(a);
}

void test_remove_shifts(void) {
    dyn_array_int_t *a = make_small_array();
    for (int i = 0; i < 5; ++i) dyn_array_int_push_back(a, i + 1); /* [1,2,3,4,5] */
    TEST_ASSERT_TRUE(dyn_array_int_remove(a, 2));                  /* remove value 3 */
    /* now [1,2,4,5] */
    TEST_ASSERT_EQUAL_SIZE_T(4, dyn_array_int_size(a));
    TEST_ASSERT_EQUAL_INT(1, dyn_array_int_get(a, 0));
    TEST_ASSERT_EQUAL_INT(2, dyn_array_int_get(a, 1));
    TEST_ASSERT_EQUAL_INT(4, dyn_array_int_get(a, 2));
    TEST_ASSERT_EQUAL_INT(5, dyn_array_int_get(a, 3));

    /* remove at OOB should fail */
    TEST_ASSERT_FALSE(dyn_array_int_remove(a, 10));

    dyn_array_int_free(a);
}

void test_clear(void) {
    dyn_array_int_t *a = make_small_array();
    dyn_array_int_push_back(a, 10);
    dyn_array_int_push_back(a, 20);
    TEST_ASSERT_EQUAL_SIZE_T(2, dyn_array_int_size(a));
    dyn_array_int_clear(a);
    TEST_ASSERT_EQUAL_SIZE_T(0, dyn_array_int_size(a));
    /* capacity should be unchanged */
    TEST_ASSERT_TRUE(dyn_array_int_capacity(a) >= 2);
    dyn_array_int_free(a);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_init_and_properties);
    RUN_TEST(test_push_and_get_and_size);
    RUN_TEST(test_pop_back_and_empty_pop);
    RUN_TEST(test_get_oob_returns_empty_type);
    RUN_TEST(test_set_and_set_oob);
    RUN_TEST(test_insert_middle_and_front);
    RUN_TEST(test_remove_shifts);
    RUN_TEST(test_clear);

    return UNITY_END();
}
