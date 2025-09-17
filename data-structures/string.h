#ifndef _POCKET_DATA_STRUCTURES_STRING_H
#define _POCKET_DATA_STRUCTURES_STRING_H

#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"
IMPLEMENT_DYN_ARRAY(char, 0);

typedef struct string_t {
    dyn_array_char_t *data;
} string_t;

/*************************************/
/*******Constructors & Destructor*****/
/*************************************/

string_t string_create(const char *c_str) {
    string_t str;
    size_t len = strlen(c_str);
    // +1 for the nullterminated char
    str.data = dyn_array_char_init(len+1);
    for (size_t i = 0; i < len; i++) {
        dyn_array_char_push_back(str.data, c_str[i]);
    }
    dyn_array_char_push_back(str.data, '\0');
    return str;
}

string_t string_create_empty() {
    string_t str;
    str.data = dyn_array_char_init(1);
    dyn_array_char_push_back(str.data, '\0');
    return str;
}

string_t string_copy(const string_t *src_str) {
    string_t str;
    str.data = NULL;
    if (likely_branch(src_str && src_str->data)) {
        str.data = dyn_array_char_init(src_str->data->size);
        for (size_t i = 0; i < src_str->data->size; i++) {
            str.data[i] = src_str->data[i];
        }
    }
    return str;
}

void string_free(string_t *str) { dyn_array_char_free(str->data); }

/*************************************/
/**************Capacity***************/
/*************************************/

size_t string_size(const string_t *str) {
    // -1 because we are not counting '\0'f
    if (likely_branch(str && str->data) && str->data->size > 1)
        return dyn_array_char_size(str->data) - 1;
    else
        return 0;
}

int string_empty(const string_t *str) { return string_size(str) == 0; }

void string_clear(string_t *str) {
    dyn_array_char_clear(str->data);
    dyn_array_char_push_back(str->data, '\0');
}

/*************************************/
/**************Element Access*********/
/*************************************/

const char *string_get_cstr(const string_t *str) { return (const char *)str->data->data; }

char string_at(const string_t *str, size_t pos) {
    if (likely_branch(pos < string_size(str))) return dyn_array_char_get(str->data, pos);
    return '\0';
}

char string_front(const string_t *str) {
    if (likely_branch(string_size(str) > 0)) return dyn_array_char_get(str->data, 0);
    return '\0';
}

char string_back(const string_t *str) {
    if (likely_branch(string_size(str) > 0)) return dyn_array_char_get(str->data, str->data->size - 1);
    return '\0';
}

/*************************************/
/**************Modifiers***************/
/*************************************/

int string_push_back(string_t *str, char ch) { return dyn_array_char_insert(str->data, str->data->size - 1, ch); }

int string_pop_back(string_t *str) { return dyn_array_char_remove(str->data, str->data->size - 1); }

int string_append_cstr(string_t *dst_str, const char *src) {
    if (likely_branch(string_size(dst_str) >= 0)) {
        size_t dst_len = string_size(dst_str);
        size_t src_len = strlen(src);
        // Remove '\0'
        dst_str->data->size -= 1;

        for (size_t i = 0; i < src_len; i++) {
            dyn_array_char_push_back(dst_str->data, src[i]);
        }
        if (dyn_array_char_get(dst_str->data, dst_str->data->size-1) != '\0') {
            dyn_array_char_push_back(dst_str->data, '\0');
        }
    }
    return 0;
}

int string_concat(string_t *dst_str, const string_t *src_str) {
    if (likely_branch(string_size(dst_str) > 0 && string_size(src_str) > 0)) {
        return string_append_cstr(dst_str, string_get_cstr(src_str));
    }
    return 0;
}

int string_erase(string_t *str, size_t pos) {
    if (likely_branch(pos < string_size(str))) {
        return dyn_array_char_remove(str->data, pos);
    }
    return 0;
}

int string_insert(string_t *str, size_t pos, char ch) {
    if (likely_branch(pos <= string_size(str))) {
        return dyn_array_char_insert(str->data, pos, ch);
    }
    return 0;
}

/*************************************/
/**************String Operations******/
/*************************************/

int string_compare(const string_t *a, const string_t *b) { return strcmp(string_get_cstr(a), string_get_cstr(b)); }

int string_equals(const string_t *a, const string_t *b) { return string_compare(a, b) == 0; }

#endif  // _POCKET_DATA_STRUCTURES_STRING_H
