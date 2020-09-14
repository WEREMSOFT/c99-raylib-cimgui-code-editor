#include <rlgl.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "desktop.h"
#include "log/log.h"
#include "options/editor_options.h"
#include "../dstring/dynamic_string.h"
#include "editor_data.h"
#include "../dstring/dynamic_string.h"

extern editor_options_t editor_options;

static editor_data_t editor_data = {0};

void desktop_data_init(void){
    editor_data.source_files[editor_data.source_files_count++] = dstring_create_char_ptr(
            "\n"
            "#include<stdio.h>\n"
            "int main(void){\n"
            "\tprintf(\"Hello World!!\\n\")\n"
            "\treturn 0;\n"
            "}\n");

    log_init();
}

void desktop_data_fini(void){
    
    log_fini();

    while(editor_data.source_files_count--){
        dstring_free(&editor_data.source_files[editor_data.source_files_count]);
    }
}

static int input_text_callback(ImGuiInputTextCallbackData *data) {
    if((IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_ENTER)){
        char selected_text[data->BufSize];

        memset(selected_text, 0, data->BufSize);

        int local_start = data->SelectionStart < data->SelectionEnd ? data->SelectionStart : data->SelectionEnd;

        strncpy(selected_text, &data->Buf[local_start], abs(data->SelectionEnd - data->SelectionStart));
        FILE *fp;
        char path[1035];

        /* Open the command for reading. */
        fp = popen(selected_text, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }
        log_append_text(".\n");
        /* Read the output a line at a time - output it. */
        while (fgets(path, sizeof(path), fp) != NULL) {
            log_append_text(path);
        }

        /* close */
        pclose(fp);
        igSetKeyboardFocusHere(0);
    }
    return 0;
}

void desktop_draw(void){
    if(igBeginMainMenuBar()){
        if(igBeginMenu("File", true)){
            igMenuItemBool("Open", "CTRL + O", false, true);
            if(igMenuItemBool("New", "CTRL + N", editor_options.show_editor_window, true)){
                editor_options.show_editor_window = !editor_options.show_editor_window;
            }
            if(igMenuItemBool("Quit", "CTRL + Q", false, true)){
                editor_options.should_close_window = true;
            }
            igEndMenu();
        }

        if(igBeginMenu("Options", true)){
            bool is_full_screen = IsWindowFullscreen();
            if(igMenuItemBool("FullScreen", NULL, is_full_screen, true)){
                ToggleFullscreen();
            }
            if(igMenuItemBool("Show FPS", NULL, editor_options.show_fps, true)){
                editor_options.show_fps = !editor_options.show_fps;
            }
            if(igMenuItemBool("Show Demo Windos", NULL, editor_options.show_demo_window, true)){
                editor_options.show_demo_window = !editor_options.show_demo_window;
            }
            igEndMenu();
        }
        

        if(igBeginMenu("View", true)){
            if(igMenuItemBool("Log", NULL, editor_options.show_log, true)){
                editor_options.show_log = !editor_options.show_log;
            }
            igEndMenu();
        }
    }igEndMainMenuBar();

    if(editor_options.show_editor_window){
        igBegin("noname.c", &editor_options.show_editor_window, ImGuiWindowFlags_NoCollapse);
        {
            igSetWindowSizeVec2((ImVec2){520, 600}, ImGuiCond_FirstUseEver);
            
            igInputTextMultiline("##source", editor_data.source_files[0].data, editor_data.source_files[0].capacity, (ImVec2){igGetWindowWidth(), igGetWindowHeight()}, ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways, input_text_callback, NULL);
        }igEnd();
    }
    
    log_draw(&editor_options.show_log);

    if(editor_options.show_demo_window){
        bool open = true;
        igShowDemoWindow(&open);
    }

    if(editor_options.show_fps){
        DrawFPS(GetMonitorWidth(0) - 100, GetMonitorHeight(0) - 40);
    }
    
}
