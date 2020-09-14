#ifndef __DYNAMIC_STRING_H__
#define __DYNAMIC_STRING_H__

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct dstring_t {
    int64_t capacity;
    char* data;
} dstring_t;

void dstring_append_char_ptr(dstring_t* self,const char* str_to_append);
void dstring_free(dstring_t* self);
dstring_t dstring_create_char_ptr(const char* init_string);
dstring_t dstring_create_capacity(int64_t capacity);
void dstring_clear(dstring_t* self,const char* str_to_append);
#endif