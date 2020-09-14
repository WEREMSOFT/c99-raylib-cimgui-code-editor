#include "log.h"
#include "../../cimgui_impl_raylib.h"
#include <raylib.h>
#include <math.h>

static dstring_t self = {0};
static dstring_t buffer = {0};
int selection_start = 0;
int selection_end = 0;


static int input_text_callback(ImGuiInputTextCallbackData *data) {
    selection_start = data->SelectionStart <= data->SelectionEnd ? data->SelectionStart : data->SelectionEnd;
    selection_end = data->SelectionStart >= data->SelectionEnd ? data->SelectionStart : data->SelectionEnd;
    return 0;
}

void log_init(){
    self = dstring_create_capacity(1000);
    buffer = dstring_create_capacity(1000);
    dstring_append_char_ptr(&self, "Wellcome to wedit\n");
}

void log_fini(){
    dstring_free(&self);
}

void log_draw(bool* is_open){
    if(!*is_open) return;
    igBegin("log", is_open, 0);
    {
        igSetWindowPosVec2((ImVec2){100.0f, 100.0f}, ImGuiCond_Once);
        igSetWindowSizeVec2((ImVec2){520, 600}, ImGuiCond_Once);

        if(igInputTextMultiline("log", self.data, self.capacity, (ImVec2){igGetWindowWidth(), igGetWindowHeight()}, ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways, input_text_callback, NULL)){
            if((IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_ENTER)){
                char selected_text[strlen(self.data)];

                memset(selected_text, 0, strlen(self.data));

                strncpy(selected_text, &self.data[selection_start], selection_end - selection_start);
                FILE *fp;
                char path[1035];

                /* Open the command for reading. */
                fp = popen(selected_text, "r");
                if (fp == NULL) {
                    printf("Failed to run command\n" );
                    exit(1);
                }
                dstring_append_char_ptr(&self, "\n");
                /* Read the output a line at a time - output it. */
                while (fgets(path, sizeof(path), fp) != NULL) {
                    log_append_text(path);
                }

                /* close */
                pclose(fp);
                igSetKeyboardFocusHere(0);
            }
        }

    }igEnd();
}

void log_append_text(char* text){
    dstring_append_char_ptr(&self, text);
}