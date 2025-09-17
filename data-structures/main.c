#include <stdio.h>


/*************************************/
/***********Dynamic Array*************/
/*************************************/
#include "dynamic_array.h"

// Here are some examples about how to create types with many qualifiers
typedef const char *const_char_ptr;
IMPLEMENT_DYN_ARRAY(const_char_ptr, NULL);

// An array of function pointers
typedef void (*error_handler_t)(const char *msg);
IMPLEMENT_DYN_ARRAY(error_handler_t, NULL);

// Simple int array
IMPLEMENT_DYN_ARRAY(int, 0);
void dynamic_array_examples(){

    struct dyn_array_int_t *arr = dyn_array_int_init(4);

    // Put some numbers in the array
    dyn_array_int_push_back(arr, 1);
    dyn_array_int_push_back(arr, 2);
    dyn_array_int_push_back(arr, 3);
    dyn_array_int_push_back(arr, 4);

    // Overwrite element at position 2
    dyn_array_int_set(arr, 2, -1);

    // Print the array
    for (size_t i = 0; i < dyn_array_int_size(arr); i++) {
        printf("%d ", dyn_array_int_get(arr, i));
    }
    printf("\n");

    dyn_array_int_free(arr);

}

/*************************************/
/*************Strings*****************/
/*************************************/
#include "string.h"
#include <assert.h>

void test_string_concat() {
    printf("Running test_string_concat...\n");
    string_t s1 = string_create("Hello");
    string_t s2 = string_create(" World");
    string_concat(&s1, &s2);
    string_t s3 = string_create("Hello World");
    assert(string_equals(&s1, &s3)); // temp comparison
    string_free(&s1);
    string_free(&s2);
    string_free(&s3);
}

void test_string_append_cstr() {
    printf("Running test_string_append_cstr...\n");
    string_t s = string_create("Hello");
    string_append_cstr(&s, "!");
    string_t s2 = string_create("Hello!");
    assert(string_equals(&s, &s2));
    string_free(&s);
    string_free(&s2);
}

void test_string_insert() {
    printf("Running test_string_insert...\n");
    string_t s = string_create("Helo");
    string_insert(&s, 2, 'l'); // H + e + (insert l) + l + o
    string_t s2 = string_create("Hello");
    assert(string_equals(&s, &s2));
    string_free(&s);
    string_free(&s2);
}

void test_string_erase() {
    printf("Running test_string_erase...\n");
    string_t s = string_create("Hxello");
    string_erase(&s, 1);
    string_t s2 = string_create("Hello");
    assert(string_equals(&s, &s2));
    string_free(&s);
    string_free(&s2);
}

void test_string_compare_and_equals() {
    printf("Running test_string_compare_and_equals...\n");
    string_t s1 = string_create("abc");
    string_t s2 = string_create("abc");
    string_t s3 = string_create("abd");

    assert(string_equals(&s1, &s2));
    assert(!string_equals(&s1, &s3));
    assert(string_compare(&s1, &s3) < 0);

    string_free(&s1);
    string_free(&s2);
    string_free(&s3);
}

void test_string_empty_and_size() {
    printf("Running test_string_empty_and_size...\n");
    string_t s = string_create("");
    assert(string_empty(&s));
    string_append_cstr(&s, "abc");
    assert(!string_empty(&s));
    assert(string_size(&s) == 3);
    string_free(&s);
}

void test_string_at() {
    printf("Running test_string_at...\n");
    string_t s = string_create("Hello");
    assert(string_at(&s, 0) == 'H');
    assert(string_at(&s, 4) == 'o');
    assert(string_at(&s, 10) == '\0'); // out of range, returns safe value
    string_free(&s);
}

int string_tests() {
    printf("=== Running String Tests ===\n");
    test_string_concat();
    test_string_append_cstr();
    test_string_insert();
    test_string_erase();
    test_string_compare_and_equals();
    test_string_empty_and_size();
    test_string_at();
    printf("All tests passed!\n");
}

int main() {

    dynamic_array_examples();

    string_tests();

    return 0;
}
