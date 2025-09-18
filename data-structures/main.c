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

int main() {

    dynamic_array_examples();

    return 0;
}
