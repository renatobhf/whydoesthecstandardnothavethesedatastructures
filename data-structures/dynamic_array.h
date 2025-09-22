#ifndef _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H
#define _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

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

#define DYN_ARRAY_DECLARE(DECL_NAME, TYPE)                                             \
    typedef struct DECL_NAME##_t {                                                     \
        TYPE *data;                                                                    \
        size_t size;                                                                   \
        size_t capacity;                                                               \
    } DECL_NAME##_t;                                                                   \
                                                                                       \
    static DECL_NAME##_t *DECL_NAME##_create(size_t starting_capacity);                \
                                                                                       \
    static int DECL_NAME##_push_back(DECL_NAME##_t *dyn_array, TYPE value);            \
                                                                                       \
    static TYPE DECL_NAME##_pop_back(DECL_NAME##_t *dyn_array);                        \
                                                                                       \
    static TYPE DECL_NAME##_get(const DECL_NAME##_t *dyn_array, size_t index);         \
                                                                                       \
    static int DECL_NAME##_set(DECL_NAME##_t *dyn_array, size_t index, TYPE value);    \
                                                                                       \
    static int DECL_NAME##_insert(DECL_NAME##_t *dyn_array, size_t index, TYPE value); \
                                                                                       \
    static int DECL_NAME##_remove(DECL_NAME##_t *dyn_array, size_t index);             \
                                                                                       \
    static size_t DECL_NAME##_size(const DECL_NAME##_t *dyn_array);                    \
                                                                                       \
    static size_t DECL_NAME##_capacity(const DECL_NAME##_t *dyn_array);                \
                                                                                       \
    static void DECL_NAME##_free(DECL_NAME##_t *dyn_array);                            \
                                                                                       \
    static void DECL_NAME##_clear(DECL_NAME##_t *dyn_array);

#define DYN_ARRAY_IMPLEMENT(DECL_NAME, TYPE)                                                                         \
    static DECL_NAME##_t *DECL_NAME##_create(size_t starting_capacity) {                                             \
        DECL_NAME##_t *dyn_array = calloc(1, sizeof(DECL_NAME##_t));                                                 \
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
    static int DECL_NAME##_ensure_capacity(DECL_NAME##_t *dyn_array, size_t min_capacity) {                          \
        if (unlikely_branch(min_capacity <= dyn_array->capacity)) return 1;                                          \
        size_t new_capacity = dyn_array->capacity * DYNAMIC_ARRAY_GROWTH_FACTOR;                                     \
        TYPE *new_data = realloc(dyn_array->data, new_capacity * sizeof(TYPE));                                      \
        if (unlikely_branch(!new_data)) return 0;                                                                    \
        dyn_array->data = new_data;                                                                                  \
        dyn_array->capacity = new_capacity;                                                                          \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int DECL_NAME##_push_back(DECL_NAME##_t *dyn_array, TYPE value) {                                         \
        if (unlikely_branch(!DECL_NAME##_ensure_capacity(dyn_array, dyn_array->size + 1))) return 0;                 \
        dyn_array->data[(dyn_array->size)++] = value;                                                                \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static TYPE DECL_NAME##_pop_back(DECL_NAME##_t *dyn_array) {                                                     \
        if (unlikely_branch(dyn_array->size == 0)) {                                                                 \
            return (TYPE)0;                                                                                          \
        }                                                                                                            \
        return dyn_array->data[--(dyn_array->size)];                                                                 \
    }                                                                                                                \
                                                                                                                     \
    static TYPE DECL_NAME##_get(const DECL_NAME##_t *dyn_array, size_t index) {                                      \
        if (unlikely_branch(index >= dyn_array->size)) {                                                             \
            return (TYPE)0;                                                                                          \
        }                                                                                                            \
        return dyn_array->data[index];                                                                               \
    }                                                                                                                \
                                                                                                                     \
    static int DECL_NAME##_set(DECL_NAME##_t *dyn_array, size_t index, TYPE value) {                                 \
        if (unlikely_branch(index >= dyn_array->size)) return 0;                                                     \
        dyn_array->data[index] = value;                                                                              \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int DECL_NAME##_insert(DECL_NAME##_t *dyn_array, size_t index, TYPE value) {                              \
        if (unlikely_branch(index > dyn_array->size)) return 0; /* Out of bounds */                                  \
        if (unlikely_branch(!DECL_NAME##_ensure_capacity(dyn_array, dyn_array->size + 1))) return 0;                 \
        /* Shift elements to the right */                                                                            \
        memmove(&dyn_array->data[index + 1], &dyn_array->data[index], (dyn_array->size - index) * sizeof(TYPE));     \
        dyn_array->data[index] = value;                                                                              \
        dyn_array->size++;                                                                                           \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int DECL_NAME##_remove(DECL_NAME##_t *dyn_array, size_t index) {                                          \
        if (unlikely_branch(index >= dyn_array->size)) return 0;                                                     \
        memmove(&dyn_array->data[index], &dyn_array->data[index + 1], (dyn_array->size - index - 1) * sizeof(TYPE)); \
        dyn_array->size--;                                                                                           \
        return 1;                                                                                                    \
    }                                                                                                                \
                                                                                                                     \
    static size_t DECL_NAME##_size(const DECL_NAME##_t *dyn_array) { return dyn_array->size; }                       \
                                                                                                                     \
    static size_t DECL_NAME##_capacity(const DECL_NAME##_t *dyn_array) { return dyn_array->capacity; }               \
                                                                                                                     \
    static void DECL_NAME##_free(DECL_NAME##_t *dyn_array) {                                                         \
        if (likely_branch(dyn_array)) {                                                                              \
            if (likely_branch(dyn_array->data)) {                                                                    \
                free(dyn_array->data);                                                                               \
            }                                                                                                        \
            free(dyn_array);                                                                                         \
        }                                                                                                            \
    }                                                                                                                \
                                                                                                                     \
    static void DECL_NAME##_clear(DECL_NAME##_t *dyn_array) { dyn_array->size = 0; }

#ifdef __cplusplus
}
#endif
#endif  // _POCKET_DATA_STRUCTURES_DYNAMIC_ARRAY_H
