#ifndef __LOG_H__
#define __LOG_H__
#include <stdbool.h>
#include "../../dstring/dynamic_string.h"
void log_draw(bool* is_open);
void log_init();
void log_fini();
void log_append_text(char* text); 
#endif
