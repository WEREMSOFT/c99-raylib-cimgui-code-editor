#ifndef __EDITOR_OPTIONS_H__
#define __EDITOR_OPTIONS_H__

#include <stdbool.h>

typedef struct editor_options_t {
    bool show_editor_window;
    bool should_close_window;
    bool show_fps;
    bool show_log;
    bool show_demo_window;
} editor_options_t;

#endif