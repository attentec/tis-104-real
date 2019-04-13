#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "display_sw.h"
#include "panic.h"
#include "pin.h"
#include "platform.h"
#include "surface_sdl.h"
#include "uart.h"
#include "uart_stdio.h"

#define MAX_SIZE 4

struct platform_t platform;

static int grid_cols;
static int grid_rows;
static int surface_width;
static int surface_height;

static SDL_Window *window;
static SDL_Surface *window_surface;
static SDL_Surface *canvas_surfaces[MAX_SIZE][MAX_SIZE];
static struct surface_t display_surfaces[MAX_SIZE][MAX_SIZE];
static struct display_t displays[MAX_SIZE][MAX_SIZE];
static struct pin_t led;
static struct uart_t uart;

const int small_size = 176;
const int large_size = 220;

void platform_init(enum orientation_t orientation, int cols, int rows, int argc, char **argv)
{
    (void) argc;
    (void) argv;
    grid_cols = cols;
    grid_rows = rows;
    switch (orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
    case ORIENTATION_RIBBON_TOP:
        surface_width = small_size;
        surface_height = large_size;
        break;
    case ORIENTATION_RIBBON_LEFT:
    case ORIENTATION_RIBBON_RIGHT:
        surface_width = large_size;
        surface_height = small_size;
        break;
    }
    int window_width = cols * surface_width;
    int window_height = rows * surface_height;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "tis-104-real sim",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        window_width,
        window_height,
        SDL_WINDOW_HIDDEN);
    window_surface = SDL_GetWindowSurface(window);

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            canvas_surfaces[c][r] = SDL_CreateRGBSurface(
                0,
                surface_width,
                surface_height,
                // RGB565
                16,
                0x0000F800,
                0x000007E0,
                0x0000001F,
                0x00000000);
            surface_sdl_init(&display_surfaces[c][r], canvas_surfaces[c][r]);
            display_sw_init(&displays[c][r], &display_surfaces[c][r]);
        }
    }

    led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);
    uart_stdio_init(&uart);

    platform.display = &displays[0][0];
    platform.led = &led;
    platform.uart = &uart;
}

struct display_t *platform_get_display(int col, int row)
{
    if (col < 0 || row < 0 || col >= grid_cols || row >= grid_rows) {
        panic();
    }
    return &displays[col][row];
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
    for (int c = 0; c < grid_cols; c++) {
        for (int r = 0; r < grid_rows; r++) {
            SDL_Rect dst_pos;
            dst_pos.x = c * surface_width;
            dst_pos.y = r * surface_height;
            SDL_BlitSurface(canvas_surfaces[c][r], NULL, window_surface, &dst_pos);
        }
    }
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
    return true;
}

void platform_deinit()
{
    for (int c = 0; c < grid_cols; c++) {
        for (int r = 0; r < grid_rows; r++) {
            SDL_FreeSurface(canvas_surfaces[c][r]);
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}
