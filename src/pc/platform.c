#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "display.h"
#include "display_sdl.h"
#include "pin.h"
#include "platform.h"
#include "uart.h"
#include "uart_stdio.h"

struct platform_t platform;

static struct display_t display;
static struct pin_t led;
static struct uart_t uart;

static SDL_Window *window;
static SDL_Surface *window_surface;
static SDL_Surface *canvas_surface;

static const int rows = 220;
static const int cols = 176;

void platform_init(enum orientation_t orientation)
{
    int window_width;
    int window_height;
    switch (orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
    case ORIENTATION_RIBBON_TOP:
        window_width = cols;
        window_height = rows;
        break;
    case ORIENTATION_RIBBON_LEFT:
    case ORIENTATION_RIBBON_RIGHT:
        window_width = rows;
        window_height = cols;
        break;
    }
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "tis-104-real sim",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_HIDDEN);
    window_surface = SDL_GetWindowSurface(window);
    canvas_surface = SDL_CreateRGBSurface(
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

    led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);
    uart_stdio_init(&uart);

    platform.display = &display;
    platform.led = &led;
    platform.uart = &uart;
}

void platform_begin(void)
{
    SDL_ShowWindow(window);
}

bool platform_loop(void)
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                return false;
            }
        }
    }
    SDL_BlitSurface(canvas_surface, NULL, window_surface, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
    return true;
}

void platform_deinit()
{
    SDL_FreeSurface(canvas_surface);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
