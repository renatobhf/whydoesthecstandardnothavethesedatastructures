#ifndef _POCKET_HASH_MAP_H
#define _POCKET_HASH_MAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASH_MAP_MAX_LOAD_FACTOR
#define HASH_MAP_MAX_LOAD_FACTOR 0.75
#endif

#ifndef HASH_MAP_GROWTH_FACTOR
#define HASH_MAP_GROWTH_FACTOR 2
#endif

#define HASH_MAP_DECLARE(DECL_NAME, KEY_TYPE, VALUE_TYPE)                                                          \
    typedef enum entry_status_t {                                                                                  \
        FREE, /* free will be 0, which is default allocated with calloc */                                         \
        OCCUPIED,                                                                                                  \
        TOMBSTONE,                                                                                                 \
    } entry_status_t;                                                                                              \
    /* Default FNV-1a hash */                                                                                      \
    static inline uint64_t fnv_1a_hash_bytes(const void *data, size_t len) {                                       \
        const uint8_t *ptr = (const uint8_t *)data;                                                                \
        uint64_t hash = 1469598103934665603ULL;                                                                    \
        for (size_t i = 0; i < len; i++) {                                                                         \
            hash ^= ptr[i];                                                                                        \
            hash *= 1099511628211ULL;                                                                              \
        }                                                                                                          \
        return hash;                                                                                               \
    }                                                                                                              \
    /* Provides some default hash functions definitions */                                                         \
    static inline uint64_t default_hash_uint64(uint64_t key) { return fnv_1a_hash_bytes(&key, sizeof(uint64_t)); } \
    static inline uint64_t default_hash_int(int key) { return fnv_1a_hash_bytes(&key, sizeof(int)); }              \
    static inline uint64_t default_hash_double(double key) { return fnv_1a_hash_bytes(&key, sizeof(double)); }     \
    static inline uint64_t default_hash_cstr(const char *key) { return fnv_1a_hash_bytes(key, strlen(key)); }      \
    typedef uint64_t (*hash_fn_##KEY_TYPE##_t)(KEY_TYPE key);                                                      \
                                                                                                                   \
    typedef struct {                                                                                               \
        entry_status_t status;                                                                                     \
        KEY_TYPE key;                                                                                              \
        VALUE_TYPE value;                                                                                          \
    } DECL_NAME##_entry_t;                                                                                         \
                                                                                                                   \
    typedef struct {                                                                                               \
        size_t capacity;                                                                                           \
        size_t occupancy;                                                                                          \
        DECL_NAME##_entry_t *entries;                                                                              \
        bool (*keys_equal_fn)(KEY_TYPE, KEY_TYPE);                                                                 \
        hash_fn_##KEY_TYPE##_t hash_fn;                                                                            \
    } DECL_NAME##_t;                                                                                               \
                                                                                                                   \
    DECL_NAME##_t DECL_NAME##_create(size_t initial_capacity, bool (*keys_equal_fn)(KEY_TYPE, KEY_TYPE),           \
                                     hash_fn_##KEY_TYPE##_t hash_fn);                                              \
    void DECL_NAME##_free(DECL_NAME##_t *map);                                                                     \
    VALUE_TYPE *DECL_NAME##_find(DECL_NAME##_t *map, KEY_TYPE key);                                                \
    bool DECL_NAME##_insert(DECL_NAME##_t *map, KEY_TYPE key, VALUE_TYPE value);                                   \
    bool DECL_NAME##_erase(DECL_NAME##_t *map, KEY_TYPE key);                                                      \
                                                                                                                   \
    typedef struct DECL_NAME##_it_t {                                                                              \
        DECL_NAME##_t *map;                                                                                        \
        size_t index;                                                                                              \
    } DECL_NAME##_it_t;                                                                                            \
                                                                                                                   \
    /* Initialize iterator (points to first valid element if any) */                                               \
    DECL_NAME##_it_t DECL_NAME##_it_begin(DECL_NAME##_t *map);                                                     \
                                                                                                                   \
    /* Advance to next valid element. Returns false if no more elements. */                                        \
    bool DECL_NAME##_it_next(DECL_NAME##_it_t *it);                                                                \
                                                                                                                   \
    /* Access key and value at current iterator position */                                                        \
    KEY_TYPE DECL_NAME##_it_key(DECL_NAME##_it_t *it);                                                             \
    VALUE_TYPE DECL_NAME##_it_value(DECL_NAME##_it_t *it);

#define HASH_MAP_IMPLEMENT(DECL_NAME, KEY_TYPE, VALUE_TYPE)                                                           \
                                                                                                                      \
    static DECL_NAME##_entry_t *find_entry_##KEY_TYPE##_##VALUE_TYPE(                                                 \
        DECL_NAME##_entry_t *entries, size_t capacity, KEY_TYPE key, bool (*keys_equal_fn)(KEY_TYPE, KEY_TYPE),       \
        hash_fn_##KEY_TYPE##_t hash_fn) {                                                                             \
        uint64_t hash = hash_fn(key);                                                                                 \
        size_t index = hash % capacity;                                                                               \
        DECL_NAME##_entry_t *tombstone = NULL;                                                                        \
        while (1) {                                                                                                   \
            DECL_NAME##_entry_t *entry = &entries[index];                                                             \
            switch (entry->status) {                                                                                  \
                case (OCCUPIED):                                                                                      \
                    if (keys_equal_fn(entry->key, key)) {                                                             \
                        return entry;                                                                                 \
                    }                                                                                                 \
                    break;                                                                                            \
                case TOMBSTONE:                                                                                       \
                    tombstone = (tombstone == NULL) ? entry : tombstone;                                              \
                    break;                                                                                            \
                case FREE:                                                                                            \
                    return tombstone ? tombstone : entry;                                                             \
            }                                                                                                         \
            index = (index + 1) % capacity;                                                                           \
        }                                                                                                             \
    }                                                                                                                 \
                                                                                                                      \
    DECL_NAME##_t DECL_NAME##_create(size_t initial_capacity, bool (*keys_equal_fn)(KEY_TYPE, KEY_TYPE),              \
                                     hash_fn_##KEY_TYPE##_t hash_fn) {                                                \
        DECL_NAME##_t map = {0};                                                                                      \
        map.capacity = 1;                                                                                             \
        while (map.capacity < initial_capacity) map.capacity <<= 1;                                                   \
        map.entries = calloc(map.capacity, sizeof(DECL_NAME##_entry_t));                                              \
        map.keys_equal_fn = keys_equal_fn;                                                                            \
        map.hash_fn = hash_fn;                                                                                        \
        return map;                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    void DECL_NAME##_free(DECL_NAME##_t *map) {                                                                       \
        free(map->entries);                                                                                           \
        map->entries = NULL;                                                                                          \
        map->capacity = 0;                                                                                            \
        map->occupancy = 0;                                                                                           \
    }                                                                                                                 \
                                                                                                                      \
    static bool DECL_NAME##_rehash(DECL_NAME##_t *map, size_t new_capacity) {                                         \
        DECL_NAME##_entry_t *new_entries = calloc(new_capacity, sizeof(DECL_NAME##_entry_t));                         \
        if (!new_entries) return false;                                                                               \
        /* Copy old entries into newly allocated entry array */                                                       \
        for (size_t i = 0; i < map->capacity; i++) {                                                                  \
            DECL_NAME##_entry_t *entry = &map->entries[i];                                                            \
            if (entry->status != OCCUPIED) {                                                                          \
                continue;                                                                                             \
            }                                                                                                         \
            DECL_NAME##_entry_t *dest = find_entry_##KEY_TYPE##_##VALUE_TYPE(new_entries, new_capacity, entry->key,   \
                                                                             map->keys_equal_fn, map->hash_fn);       \
            dest->key = entry->key;                                                                                   \
            dest->value = entry->value;                                                                               \
            dest->status = entry->status;                                                                             \
        }                                                                                                             \
                                                                                                                      \
        free(map->entries);                                                                                           \
        map->entries = new_entries;                                                                                   \
        map->capacity = new_capacity;                                                                                 \
        return true;                                                                                                  \
    }                                                                                                                 \
                                                                                                                      \
    VALUE_TYPE *DECL_NAME##_find(DECL_NAME##_t *map, KEY_TYPE key) {                                                  \
        if (map->occupancy == 0) return NULL;                                                                         \
        DECL_NAME##_entry_t *entry =                                                                                  \
            find_entry_##KEY_TYPE##_##VALUE_TYPE(map->entries, map->capacity, key, map->keys_equal_fn, map->hash_fn); \
        return entry->status == OCCUPIED ? &entry->value : NULL;                                                      \
    }                                                                                                                 \
                                                                                                                      \
    bool DECL_NAME##_insert(DECL_NAME##_t *map, KEY_TYPE key, VALUE_TYPE value) {                                     \
        if (map->occupancy + 1 > map->capacity * HASH_MAP_MAX_LOAD_FACTOR) {                                          \
            if (!DECL_NAME##_rehash(map, map->capacity * HASH_MAP_GROWTH_FACTOR)) return false;                       \
        }                                                                                                             \
        DECL_NAME##_entry_t *entry =                                                                                  \
            find_entry_##KEY_TYPE##_##VALUE_TYPE(map->entries, map->capacity, key, map->keys_equal_fn, map->hash_fn); \
        if (entry->status == FREE) map->occupancy++;                                                                  \
        entry->key = key;                                                                                             \
        entry->value = value;                                                                                         \
        entry->status = OCCUPIED;                                                                                     \
        return true;                                                                                                  \
    }                                                                                                                 \
                                                                                                                      \
    bool DECL_NAME##_erase(DECL_NAME##_t *map, KEY_TYPE key) {                                                        \
        if (map->occupancy == 0) return false;                                                                        \
        DECL_NAME##_entry_t *entry =                                                                                  \
            find_entry_##KEY_TYPE##_##VALUE_TYPE(map->entries, map->capacity, key, map->keys_equal_fn, map->hash_fn); \
        if (entry->status == FREE) return false;                                                                      \
        entry->status = TOMBSTONE;                                                                                    \
        map->occupancy--;                                                                                             \
        return true;                                                                                                  \
    }                                                                                                                 \
                                                                                                                      \
    DECL_NAME##_it_t DECL_NAME##_it_begin(DECL_NAME##_t *map) {                                                       \
        DECL_NAME##_it_t it = {map, 0};                                                                               \
        /* Advance until we find a non-empty slot */                                                                  \
        while (it.index < map->capacity && map->entries[it.index].key == (KEY_TYPE)0) {                               \
            it.index++;                                                                                               \
        }                                                                                                             \
        return it;                                                                                                    \
    }                                                                                                                 \
                                                                                                                      \
    bool DECL_NAME##_it_next(DECL_NAME##_it_t *it) {                                                                  \
        if (!it->map) return false;                                                                                   \
        it->index++;                                                                                                  \
        while (it->index < it->map->capacity && it->map->entries[it->index].key == (KEY_TYPE)0) {                     \
            it->index++;                                                                                              \
        }                                                                                                             \
        return it->index < it->map->capacity;                                                                         \
    }                                                                                                                 \
                                                                                                                      \
    KEY_TYPE DECL_NAME##_it_key(DECL_NAME##_it_t *it) { return it->map->entries[it->index].key; }                     \
                                                                                                                      \
    VALUE_TYPE DECL_NAME##_it_value(DECL_NAME##_it_t *it) { return it->map->entries[it->index].value; }

#endif /* _POCKET_HASH_MAP_H */
