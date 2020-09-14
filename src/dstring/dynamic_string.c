#include "dynamic_string.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void dstring_clear(dstring_t* self,const char* str_to_append){
    memset(self->data, 0, self->capacity);
}

void dstring_append_char_ptr(dstring_t* self,const char* str_to_append){
    if (strlen(self->data) + strlen(str_to_append) >= self->capacity){
        // convention, when we run out of space, we duplicate the size of the string.
        self->data = (char *)realloc(self->data, self->capacity * 2);
        self->capacity *= 2;
    }

    strcat(self->data, str_to_append);
}

void dstring_free(dstring_t* self){
    free(self->data);
    self->capacity = 0;
}

dstring_t dstring_create_char_ptr(const char* init_string){
    dstring_t return_value = {0};

    return_value.capacity = strlen(init_string);

    return_value.data = (char *)calloc(return_value.capacity, sizeof(return_value.data[0]));
    
    dstring_append_char_ptr(&return_value, init_string);

    return return_value;
}

dstring_t dstring_create_capacity(int64_t capacity){
    dstring_t return_value = {0};

    return_value.capacity = capacity;

    return_value.data = (char *)calloc(return_value.capacity, sizeof(return_value.data[0]));

    return return_value;
}