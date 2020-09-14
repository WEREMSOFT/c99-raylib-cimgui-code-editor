#ifndef __EDITOR_DATA_H__
#define __EDITOR_DATA_H__
#include "../dstring/dynamic_string.h"
#include <stdint.h>

typedef struct editor_data_t {
    uint8_t source_files_count;
    dstring_t source_files[256];
} editor_data_t;

#endif