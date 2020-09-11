#include <rlgl.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "editor.h"
#include "editor_options.h"

extern editor_options_t editor_options;

int input_text_callback(ImGuiInputTextCallbackData *data) {
    char selected_text[data->BufSize];

    memset(selected_text, 0, data->BufSize);

    int local_start = data->SelectionStart < data->SelectionEnd ? data->SelectionStart : data->SelectionEnd;

    strncpy(selected_text, &data->Buf[local_start], abs(data->SelectionEnd - data->SelectionStart));

    // printf("callback called!! %s %d\n", selected_text, data->SelectionEnd - data->SelectionStart);
    if((IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_ENTER)){
        FILE *fp;
        char path[1035];

        /* Open the command for reading. */
        fp = popen(selected_text, "r");
        if (fp == NULL) {
            printf("Failed to run command\n" );
            exit(1);
        }

        /* Read the output a line at a time - output it. */
        while (fgets(path, sizeof(path), fp) != NULL) {
            printf("%s", path);
        }

        /* close */
        pclose(fp);
    }
}

void draw_editor(void){
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
        igEndMainMenuBar();
    }

    if(editor_options.show_editor_window){
        igBegin("noname.c", true, 0);
        {
            igSetWindowSizeVec2((ImVec2){520, 600}, ImGuiCond_FirstUseEver);
            static char text[1024 * 16] =
            "\n"
            "#include<stdio.h>\n"
            "int main(void){\n"
            "\tprintf(\"Hello World!!\\n\")\n"
            "\treturn 0;\n"
            "}\n";

            igInputTextMultiline("##source", text, 1024*16, (ImVec2){igGetWindowWidth(), igGetWindowHeight()}, ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CallbackAlways, input_text_callback, NULL);

            if((IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_ENTER)){
                igSetKeyboardFocusHere(0);
            }

        }igEnd();
    }
    if(editor_options.show_demo_window){
        igShowDemoWindow(true);
    }

    if(editor_options.show_fps){
        DrawFPS(GetMonitorWidth(0) - 100, GetMonitorHeight(0) - 40);
    }
    
}
