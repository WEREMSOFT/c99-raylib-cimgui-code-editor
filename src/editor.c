#include <rlgl.h>
#include <raylib.h>
#include "editor.h"
#include "editor_options.h"

extern editor_options_t editor_options;

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

            
            igInputTextMultiline("##source", text, 1024*16, (ImVec2){igGetWindowWidth(), igGetWindowHeight()}, ImGuiInputTextFlags_AllowTabInput, NULL, NULL);
        }igEnd();
    }

    if(editor_options.show_fps){
        DrawFPS(GetMonitorWidth(0) - 100, GetMonitorHeight(0) - 40);
    }
    
}
