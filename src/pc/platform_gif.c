#include <gif_lib.h>
#include <stdio.h>
#include <stdlib.h>

#include "display_sw.h"
#include "panic.h"
#include "pin.h"
#include "platform.h"
#include "surface_gif.h"
#include "uart_stdio.h"

#define MAX_SIZE 4
#define MAX_COLORS 4

struct platform_t platform;

static int grid_cols;
static int grid_rows;
static int tile_width;
static int tile_height;
static int image_width;
static int image_height;

static int total_frames;
static int current_frame;
static GifFileType* gif;
static ColorMapObject *color_map;
static int error;
static GifPixelType *pixels;
static struct surface_t display_surfaces[MAX_SIZE][MAX_SIZE];
static struct display_t displays[MAX_SIZE][MAX_SIZE];
static struct pin_t led;
static struct uart_t uart;

const int small_size = 176;
const int large_size = 220;

static void panic_gif(void)
{
    printf("%s\n", GifErrorString(error));
    panic();
}

void platform_init(enum orientation_t orientation, int cols, int rows, int argc, char **argv)
{
    (void) argc;
    (void) argv;
    grid_cols = cols;
    grid_rows = rows;
    switch (orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
    case ORIENTATION_RIBBON_TOP:
        tile_width = small_size;
        tile_height = large_size;
        break;
    case ORIENTATION_RIBBON_LEFT:
    case ORIENTATION_RIBBON_RIGHT:
        tile_width = large_size;
        tile_height = small_size;
        break;
    }
    image_width = cols * tile_width;
    image_height = rows * tile_height;
    if ((gif = EGifOpenFileName("tis-104-real.gif", false, &error)) == NULL) {
        panic_gif();
    }
    if ((color_map = GifMakeMapObject(MAX_COLORS, NULL)) == NULL) {
        panic();
    }
    color_map->Colors[0].Red   = 0x00;
    color_map->Colors[0].Green = 0x00;
    color_map->Colors[1].Blue  = 0x00;
    color_map->Colors[1].Red   = 0xFF;
    color_map->Colors[1].Green = 0xFF;
    color_map->Colors[1].Blue  = 0xFF;
    color_map->Colors[2].Red   = 0xAA;
    color_map->Colors[2].Green = 0xAA;
    color_map->Colors[2].Blue  = 0xAA;
    if (EGifPutScreenDesc(gif, image_width, image_height, MAX_COLORS, 0, color_map) != GIF_OK) {
        panic();
    }
    pixels = malloc(image_width * image_height);
    total_frames = 1;
    current_frame = 0;

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            surface_gif_init(&display_surfaces[c][r], pixels, c*tile_width, r*tile_height, tile_width, tile_height);
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
}

bool platform_loop(void)
{
    if (current_frame >= total_frames) {
        return false;
    }
    if (EGifPutImageDesc(gif, 0, 0, image_width, image_height, false, color_map) != GIF_OK) {
        panic();
    }
    for (int y = 0; y < image_height; y++) {
        if (EGifPutLine(gif, &pixels[y * image_width], image_width) != GIF_OK) {
            panic();
        }
    }
    current_frame++;
    return true;
}

void platform_deinit()
{
    free(pixels);
    if (EGifCloseFile(gif, &error) != GIF_OK) {
        panic_gif();
    }
}
