#ifndef _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H
#define _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H

#include <stdlib.h>
#include <string.h>

#ifndef DYNAMIC_ARRAY_GROWTH_FACTOR
#define DYNAMIC_ARRAY_GROWTH_FACTOR 2
#endif

#if defined(__GNUC__) || defined(__clang__)
#ifndef likely_branch
#define likely_branch(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely_branch
#define unlikely_branch(x) __builtin_expect(!!(x), 0)
#endif
#else
// Fallback for other compilers
#ifndef likely_branch
#define likely_branch(x) (x)
#endif

#ifndef unlikely_branch
#define unlikely_branch(x) (x)
#endif
#endif

#define IMPLEMENT_DYN_ARRAY(TYPE, EMPTY_TYPE)                                                                        \
    typedef struct dyn_array_##TYPE##_t {                                                                            \
        TYPE *data;                                                                                                  \
        size_t size;                                                                                                 \
        size_t capacity;                                                                                             \
    } dyn_array_##TYPE##_t;                                                                                          \
                                                                                                                     \
    static dyn_array_##TYPE##_t *dyn_array_##TYPE##_init(size_t starting_capacity) {                                 \
        dyn_array_##TYPE##_t *dyn_array = calloc(1, sizeof(dyn_array_##TYPE##_t));                                   \
        if (unlikely_branch(!dyn_array)) return NULL;                                                                \
        dyn_array->data = malloc(starting_capacity * sizeof(TYPE));                                                  \
        if (unlikely_branch(!dyn_array->data)) {                                                                     \
            free(dyn_array);                                                                                         \
            return NULL;                                                                                             \
        }                                                                                                            \
        dyn_array->size = 0;                                                                                         \
        dyn_array->capacity = starting_capacity;                                                                     \
        return dyn_array;                                                                                            \
    }                                                                                                                \
                                                                                                                     \
    static int dyn_array_##TYPE##_ensure_capacity(dyn_array_##TYPE##_t *dyn_array, size_t min_capacity) {            \
        if (unlikely_branch(min_capacity <= dyn_array->capacity)) return 1;                                          \
        size_t new_capacity = dyn_array->capacity * 2;                                                               \
        TYPE *new_data = realloc(dyn_array->data, new_capacity * sizeof(TYPE));                                      \
        if (unlikely_branch(!new_data)) return 0;                                                                    \
        dyn_array->data = new_data;                                                                                  \
        dyn_array->capacity = new_capacity;                                                                          \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int dyn_array_##TYPE##_push_back(dyn_array_##TYPE##_t *dyn_array, TYPE value) {                           \
        if (unlikely_branch(!dyn_array_##TYPE##_ensure_capacity(dyn_array, dyn_array->size + 1))) return 0;          \
        dyn_array->data[(dyn_array->size)++] = value;                                                                \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static TYPE dyn_array_##TYPE##_pop_back(dyn_array_##TYPE##_t *dyn_array) {                                       \
        if (unlikely_branch(dyn_array->size == 0)) {                                                                 \
            return EMPTY_TYPE;                                                                                       \
        }                                                                                                            \
        return dyn_array->data[--(dyn_array->size)];                                                                 \
    }                                                                                                                \
                                                                                                                     \
    static TYPE dyn_array_##TYPE##_get(const dyn_array_##TYPE##_t *dyn_array, size_t index) {                        \
        if (unlikely_branch(index >= dyn_array->size)) {                                                             \
            return EMPTY_TYPE;                                                                                       \
        }                                                                                                            \
        return dyn_array->data[index];                                                                               \
    }                                                                                                                \
                                                                                                                     \
    static int dyn_array_##TYPE##_set(dyn_array_##TYPE##_t *dyn_array, size_t index, TYPE value) {                   \
        if (unlikely_branch(index >= dyn_array->size)) return 0;                                                     \
        dyn_array->data[index] = value;                                                                              \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int dyn_array_##TYPE##_insert(dyn_array_##TYPE##_t *dyn_array, size_t index, TYPE value) {                \
        if (unlikely_branch(index > dyn_array->size)) return 0; /* Out of bounds */                                  \
        if (unlikely_branch(!dyn_array_##TYPE##_ensure_capacity(dyn_array, dyn_array->size + 1))) return 0;          \
        /* Shift elements to the right */                                                                            \
        memmove(&dyn_array->data[index + 1], &dyn_array->data[index], (dyn_array->size - index) * sizeof(TYPE));     \
        dyn_array->data[index] = value;                                                                              \
        dyn_array->size++;                                                                                           \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int dyn_array_##TYPE##_remove(dyn_array_##TYPE##_t *dyn_array, size_t index) {                            \
        if (unlikely_branch(index >= dyn_array->size)) return 0;                                                     \
        memmove(&dyn_array->data[index], &dyn_array->data[index + 1], (dyn_array->size - index - 1) * sizeof(TYPE)); \
        dyn_array->size--;                                                                                           \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static size_t dyn_array_##TYPE##_size(const dyn_array_##TYPE##_t *dyn_array) { return dyn_array->size; }         \
                                                                                                                     \
    static size_t dyn_array_##TYPE##_capacity(const dyn_array_##TYPE##_t *dyn_array) { return dyn_array->capacity; } \
                                                                                                                     \
    static void dyn_array_##TYPE##_free(dyn_array_##TYPE##_t *dyn_array) {                                           \
        if (likely_branch(dyn_array && dyn_array->data)) {                                                           \
            free(dyn_array->data);                                                                                   \
            free(dyn_array);                                                                                         \
        }                                                                                                            \
    }                                                                                                                \
                                                                                                                     \
    static void dyn_array_##TYPE##_clear(dyn_array_##TYPE##_t *dyn_array) { dyn_array->size = 0; }

#endif  // _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H
