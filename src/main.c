#include <stdio.h>
#include <raylib.h>
#include <rlgl.h>

#ifdef PLATFORM_WEB
#include <emscripten/emscripten.h>
#endif

#include "cimgui_impl_raylib.h"
#define WIDTH 1024
#define HEIGHT 768

#include "desktop/desktop.h"
#include "editor_options.h"
extern editor_options_t editor_options;

#define __IMPLEMENT_STDOUT_REDIRECTION__
#include "desktop/stdout_redirection.h"

ImDrawData *draw_data;

void update_frame()
{
    ImGui_ImplRaylib_NewFrame();
    ImGui_ImplRaylib_ProcessEvent();
    igNewFrame();

    BeginDrawing();
    {
        ClearBackground(DARKBLUE);
        
        desktop_draw();

        igRender();
        draw_data = igGetDrawData();
        raylib_render_cimgui(draw_data);
    }
    EndDrawing();
}

int main(void)
{
#ifdef OS_Windows_NT
    printf("Windows dettected\n");
#elif defined OS_Linux
    printf("LINUS dettected\n");
#elif defined OS_Darwin
    printf("MacOS dettected\n");
#endif
    
    int width = WIDTH;
    int height = HEIGHT;

    stdout_redirection_init();

    // Init Raylib
    InitWindow(width, height, "This is a imgui test");
    SetTargetFPS(60);

    // ImFontAtlas_AddFontFromFileTTF();

    // cimgui variables
    struct ImGuiIO *io;

    // Initialize imgui
    igCreateContext(NULL);
    igStyleColorsDark(NULL);
    // Initialize keyboard and mouse events
    ImGui_ImplRaylib_Init();



    // Build and load the texture atlas into a texture
    // (In the examples/ app this is usually done within the ImGui_ImplXXX_Init() function from one of the demo Renderer)
    io = igGetIO();
    unsigned char *pixels = NULL;

    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, NULL);
    // At this point you've got the texture data and you need to upload that your your graphic system:
    // After we have created the texture, store its pointer/identifier (_in whichever format your engine uses_) in 'io.Fonts->TexID'.
    // This will be passed back to your via the renderer. Basically ImTextureID == void*. Read FAQ for details about ImTextureID.
    Image image = LoadImageEx((Color *)pixels, width, height);
    Texture2D texture = LoadTextureFromImage(image); //MyEngine::CreateTextureFromMemoryPixels(pixels, width, height, TEXTURE_TYPE_RGBA32)
    io->Fonts->TexID = (void *)&texture.id;
    desktop_data_init();

#ifdef PLATFORM_WEB
    emscripten_set_main_loop(update_frame, 0, 1);
#else
    while (!WindowShouldClose() && !editor_options.should_close_window)
    {
        update_frame();
    }
#endif
    desktop_data_fini();
    CloseWindow();
    std_out_redirection_fini();
    return 0;
}