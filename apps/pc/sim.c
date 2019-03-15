#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "board.h"
#include "canvas.h"
#include "code.h"
#include "display.h"
#include "display_sdl.h"
#include "dispif.h"
#include "fonts.h"
#include "icons.h"
#include "tile.h"

const char *example_program_text[CPU_MAX_PRGM_LENGTH] = {
    "  MOV 10, ACC",
    "LOOP:",
    "  SUB 1",
    "  JNZ LOOP",
    "  SWP",
    "  ADD 1",
    "  SWP",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

static const int window_width = 220;
static const int window_height = 176;

int main(void)
{
    struct display_t display;
    struct canvas_t canvas;
    struct code_t code;
    struct tile_t tile;

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(
        "tis-104-real sim",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_HIDDEN);
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
    SDL_Surface *canvas_surface = SDL_CreateRGBSurface(
        0,
        window_width,
        window_height,
        // RGB565
        16,
        0x0000F800,
        0x000007E0,
        0x0000001F,
        0x00000000);

    display_sdl_init(&display, canvas_surface);
    canvas_init(&canvas, &display, &monoblipp6x8);
    code_init(&code, example_program_text);
    tile_init(&tile, &code, &canvas);
    display_activate(&display);

    SDL_ShowWindow(window);

    for (;;) {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    break;
                }
            }
        }
        tile_draw(&tile);
        tile_read(&tile);
        tile_write(&tile);
        SDL_BlitSurface(canvas_surface, NULL, window_surface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_FreeSurface(canvas_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
