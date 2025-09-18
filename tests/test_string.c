#include "pocket_string.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

/* Helper macro to compare size_t values using Unity */
#define TEST_ASSERT_EQUAL_SIZE_T(expected, actual) \
    TEST_ASSERT_EQUAL_UINT((unsigned int)(expected), (unsigned int)(actual))

/*******************
 * Constructors
 *******************/
void test_string_create(void) {
    string_t s = string_create("Hello");
    TEST_ASSERT_NOT_NULL(s.data);
    TEST_ASSERT_EQUAL_SIZE_T(5, string_size(&s));
    TEST_ASSERT_EQUAL_STRING("Hello", string_get_cstr(&s));
    string_free(&s);
}

void test_string_create_empty(void) {
    string_t s = string_create_empty();
    TEST_ASSERT_NOT_NULL(s.data);
    TEST_ASSERT_EQUAL_SIZE_T(0, string_size(&s));
    TEST_ASSERT_TRUE(string_empty(&s));
    TEST_ASSERT_EQUAL_STRING("", string_get_cstr(&s));
    string_free(&s);
}

void test_string_copy(void) {
    string_t s1 = string_create("copy me");
    string_t s2 = string_copy(&s1);
    TEST_ASSERT_EQUAL_STRING(string_get_cstr(&s1), string_get_cstr(&s2));
    string_free(&s1);
    string_free(&s2);
}

/*******************
 * Element Access
 *******************/
void test_string_element_access(void) {
    string_t s = string_create("ABC");
    TEST_ASSERT_EQUAL_CHAR('A', string_front(&s));
    TEST_ASSERT_EQUAL_CHAR('C', string_back(&s));
    TEST_ASSERT_EQUAL_CHAR('B', string_at(&s, 1));
    TEST_ASSERT_EQUAL_CHAR('\0', string_at(&s, 10));  // Out of range must return '\0'
    string_free(&s);
}

/*******************
 * Modifiers
 *******************/
void test_string_push_pop_back(void) {
    string_t s = string_create("Hi");
    string_push_back(&s, '!');
    TEST_ASSERT_EQUAL_STRING("Hi!", string_get_cstr(&s));
    string_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("Hi", string_get_cstr(&s));
    string_free(&s);
}

void test_string_append_cstr(void) {
    string_t s = string_create("Hello");
    string_append_cstr(&s, " World");
    TEST_ASSERT_EQUAL_STRING("Hello World", string_get_cstr(&s));
    string_free(&s);
}

void test_string_concat(void) {
    string_t s1 = string_create("Hello");
    string_t s2 = string_create(" World");
    string_concat(&s1, &s2);
    TEST_ASSERT_EQUAL_STRING("Hello World", string_get_cstr(&s1));
    string_free(&s1);
    string_free(&s2);
}

void test_string_insert_erase(void) {
    string_t s = string_create("Helo");
    string_insert(&s, 2, 'l');  // Insert missing 'l'
    TEST_ASSERT_EQUAL_STRING("Hello", string_get_cstr(&s));
    string_erase(&s, 4);  // Remove 'o'
    TEST_ASSERT_EQUAL_STRING("Hell", string_get_cstr(&s));
    string_free(&s);
}

void test_string_clear(void) {
    string_t s = string_create("clear me");
    string_clear(&s);
    TEST_ASSERT_TRUE(string_empty(&s));
    TEST_ASSERT_EQUAL_STRING("", string_get_cstr(&s));
    string_free(&s);
}

/*******************
 * String Operations
 *******************/
void test_string_compare_equals(void) {
    string_t a = string_create("test");
    string_t b = string_create("test");
    string_t c = string_create("TEST");

    TEST_ASSERT_TRUE(string_equals(&a, &b));
    TEST_ASSERT_FALSE(string_equals(&a, &c));
    TEST_ASSERT_GREATER_THAN(0, string_compare(&a, &c));  // "test" > "TEST"

    string_free(&a);
    string_free(&b);
    string_free(&c);
}

/*******************
 * Edge Cases
 *******************/
void test_append_empty_string(void) {
    string_t s = string_create("Hello");
    string_append_cstr(&s, "");
    TEST_ASSERT_EQUAL_STRING("Hello", string_get_cstr(&s));
    string_free(&s);
}

void test_concat_with_empty(void) {
    string_t s1 = string_create("Hello");
    string_t s2 = string_create("");
    string_concat(&s1, &s2);
    TEST_ASSERT_EQUAL_STRING("Hello", string_get_cstr(&s1));
    string_free(&s1);
    string_free(&s2);
}

void test_insert_at_start_and_end(void) {
    string_t s = string_create("bc");
    string_insert(&s, 0, 'a');  // Insert at start
    TEST_ASSERT_EQUAL_STRING("abc", string_get_cstr(&s));
    string_insert(&s, string_size(&s), 'd');  // Insert at end
    TEST_ASSERT_EQUAL_STRING("abcd", string_get_cstr(&s));
    string_free(&s);
}

void test_erase_first_and_last_char(void) {
    string_t s = string_create("abcd");
    string_erase(&s, 0);  // erase 'a'
    TEST_ASSERT_EQUAL_STRING("bcd", string_get_cstr(&s));
    string_erase(&s, string_size(&s) - 1);  // erase last char 'd'
    TEST_ASSERT_EQUAL_STRING("bc", string_get_cstr(&s));
    string_free(&s);
}

void test_clear_empty_string(void) {
    string_t s = string_create_empty();
    string_clear(&s);
    TEST_ASSERT_EQUAL_STRING("", string_get_cstr(&s));
    TEST_ASSERT_TRUE(string_empty(&s));
    string_free(&s);
}

void test_multiple_push_and_pop(void) {
    string_t s = string_create_empty();
    string_push_back(&s, 'A');
    string_push_back(&s, 'B');
    string_push_back(&s, 'C');
    TEST_ASSERT_EQUAL_STRING("ABC", string_get_cstr(&s));

    string_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("AB", string_get_cstr(&s));
    string_pop_back(&s);
    string_pop_back(&s);
    TEST_ASSERT_EQUAL_STRING("", string_get_cstr(&s));
    TEST_ASSERT_TRUE(string_empty(&s));

    string_free(&s);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_string_create);
    RUN_TEST(test_string_create_empty);
    RUN_TEST(test_string_copy);
    RUN_TEST(test_string_element_access);
    RUN_TEST(test_string_push_pop_back);
    RUN_TEST(test_string_append_cstr);
    RUN_TEST(test_string_concat);
    RUN_TEST(test_string_insert_erase);
    RUN_TEST(test_string_clear);
    RUN_TEST(test_string_compare_equals);

    // Edge cases
    RUN_TEST(test_append_empty_string);
    RUN_TEST(test_concat_with_empty);
    RUN_TEST(test_insert_at_start_and_end);
    RUN_TEST(test_erase_first_and_last_char);
    RUN_TEST(test_clear_empty_string);
    RUN_TEST(test_multiple_push_and_pop);

    return UNITY_END();
}
