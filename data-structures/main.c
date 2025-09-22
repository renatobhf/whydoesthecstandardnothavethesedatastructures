#include <stdio.h>
#include <string.h>

/*************************************/
/***********Dynamic Array*************/
/*************************************/
#include "dynamic_array.h"

// Here are some examples about how to create types with many qualifiers
typedef const char *const_char_ptr;
DYN_ARRAY_DECLARE(da_cstr, const_char_ptr);

// An array of function pointers
typedef void (*error_handler_t)(const char *msg);
DYN_ARRAY_DECLARE(da_errhandler, error_handler_t);

// Simple int array
DYN_ARRAY_DECLARE(dyn_array_int, int);
DYN_ARRAY_IMPLEMENT(dyn_array_int, int);

void dynamic_array_examples() {
    struct dyn_array_int_t *arr = dyn_array_int_create(4);

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
#include "pocket_string.h"

void string_examples() {
    string_t str = string_create("Hello");
    string_append_cstr(&str, " World");
    string_push_back(&str, '!');
    printf("String: %s\n", string_get_cstr(&str));
    string_free(&str);
}

/*************************************/
/*************Hash Map****************/
/*************************************/

#include "hash_map.h"

/* Equality function for strings */
static bool str_equal(const char *a, const char *b) { return strcmp(a, b) == 0; }

/* Declare and implement */
HASH_MAP_DECLARE(cstr_double_map, const_char_ptr, double)
HASH_MAP_IMPLEMENT(cstr_double_map, const_char_ptr, double)

void hash_map_examples(void) {
    cstr_double_map_t map = cstr_double_map_create(8, str_equal, default_hash_cstr);

    cstr_double_map_insert(&map, "pi", 3.14159);
    cstr_double_map_insert(&map, "e", 2.71828);

    double *val = cstr_double_map_find(&map, "pi");
    if (val) printf("pi ≈ %.3f\n", *val);

    // Iterate manually
    cstr_double_map_it_t it = cstr_double_map_it_begin(&map);
    do {
        printf("Key=%s Value=%f\n", cstr_double_map_it_key(&it), cstr_double_map_it_value(&it));
    } while (cstr_double_map_it_next(&it));

    cstr_double_map_free(&map);
}

int main() {
    dynamic_array_examples();
    string_examples();
    hash_map_examples();

    return 0;
}
