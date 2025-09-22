#include "hash_map.h"
#include "unity.h"

/* ====== Declare and implement an int->int hash map ====== */
static bool int_equal(int a, int b) { return a == b; }

HASH_MAP_DECLARE(i2i_map, int, int)
HASH_MAP_IMPLEMENT(i2i_map, int, int)

/* ====== Unity test setup/teardown ====== */
void setUp(void) {}
void tearDown(void) {}

/* ====== Test cases ====== */
void test_create_and_free(void) {
    i2i_map_t map = i2i_map_create(8, int_equal, default_hash_int);
    TEST_ASSERT_NOT_NULL(map.entries);
    TEST_ASSERT_EQUAL_size_t(8, map.capacity);  // capacity should round up to power of two
    TEST_ASSERT_EQUAL_size_t(0, map.occupancy);
    i2i_map_free(&map);
    TEST_ASSERT_NULL(map.entries);
    TEST_ASSERT_EQUAL_size_t(0, map.capacity);
}

void test_insert_and_find(void) {
    i2i_map_t map = i2i_map_create(4, int_equal, default_hash_int);

    bool ok = i2i_map_insert(&map, 42, 100);
    TEST_ASSERT_TRUE(ok);
    TEST_ASSERT_EQUAL_size_t(1, map.occupancy);

    int *value = i2i_map_find(&map, 42);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL(100, *value);

    i2i_map_free(&map);
}

void test_insert_updates_existing_key(void) {
    i2i_map_t map = i2i_map_create(4, int_equal, default_hash_int);

    i2i_map_insert(&map, 10, 1);
    i2i_map_insert(&map, 10, 999);  // update value

    int *value = i2i_map_find(&map, 10);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL(999, *value);  // must have been updated

    i2i_map_free(&map);
}

void test_erase_removes_key(void) {
    i2i_map_t map = i2i_map_create(4, int_equal, default_hash_int);

    i2i_map_insert(&map, 5, 50);
    TEST_ASSERT_NOT_NULL(i2i_map_find(&map, 5));

    bool erased = i2i_map_erase(&map, 5);
    TEST_ASSERT_TRUE(erased);
    TEST_ASSERT_NULL(i2i_map_find(&map, 5));

    // erase non-existing key
    erased = i2i_map_erase(&map, 123);
    TEST_ASSERT_FALSE(erased);

    i2i_map_free(&map);
}

void test_insert_triggers_rehash(void) {
    i2i_map_t map = i2i_map_create(2, int_equal, default_hash_int);
    size_t initial_capacity = map.capacity;

    i2i_map_insert(&map, 1, 10);
    i2i_map_insert(&map, 2, 20);
    i2i_map_insert(&map, 3, 30);  // should trigger rehash when load factor > 0.75

    TEST_ASSERT_TRUE(map.capacity > initial_capacity);
    TEST_ASSERT_EQUAL(3, map.occupancy);

    int *v1 = i2i_map_find(&map, 1);
    int *v2 = i2i_map_find(&map, 2);
    int *v3 = i2i_map_find(&map, 3);
    TEST_ASSERT_EQUAL(10, *v1);
    TEST_ASSERT_EQUAL(20, *v2);
    TEST_ASSERT_EQUAL(30, *v3);

    i2i_map_free(&map);
}

void test_iterator_traverses_all_entries(void) {
    i2i_map_t map = i2i_map_create(4, int_equal, default_hash_int);
    i2i_map_insert(&map, 1, 10);
    i2i_map_insert(&map, 2, 20);
    i2i_map_insert(&map, 3, 30);

    i2i_map_it_t it = i2i_map_it_begin(&map);
    int seen_keys[3] = {0};
    int count = 0;

    // Iterate manually
    do {
        seen_keys[count++] = i2i_map_it_key(&it);
    } while (i2i_map_it_next(&it));

    TEST_ASSERT_EQUAL(3, count);
    TEST_ASSERT_TRUE(seen_keys[0] != 0);
    TEST_ASSERT_TRUE(seen_keys[1] != 0);
    TEST_ASSERT_TRUE(seen_keys[2] != 0);

    i2i_map_free(&map);
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_create_and_free);
    RUN_TEST(test_insert_and_find);
    RUN_TEST(test_insert_updates_existing_key);
    RUN_TEST(test_erase_removes_key);
    RUN_TEST(test_insert_triggers_rehash);
    RUN_TEST(test_iterator_traverses_all_entries);

    return UNITY_END();
}
