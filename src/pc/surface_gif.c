#include <gif_lib.h>

#include "display.h"
#include "panic.h"
#include "surface.h"
#include "surface_gif.h"

#define BLACK RGB888_TO_RGB565(0x000000ul)
#define WHITE RGB888_TO_RGB565(0xFFFFFFul)
#define GRAY  RGB888_TO_RGB565(0xAAAAAAul)

static GifPixelType find_color(uint16_t color);

void surface_gif_init(struct surface_t *surface, GifPixelType *pixels, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    surface->pixels = pixels;
    surface->x = x;
    surface->y = y;
    surface->w = w;
    surface->h = h;
}

uint8_t surface_get_width(struct surface_t *surface)
{
    return surface->w;
}

uint8_t surface_get_height(struct surface_t *surface)
{
    return surface->h;
}

void surface_set_pixel(struct surface_t *surface, uint8_t x, uint8_t y, uint16_t color)
{
    uint32_t pixel_x = surface->x + x;
    uint32_t pixel_y = surface->y + y;
    uint32_t index = (pixel_y * surface->w) + pixel_x;
    surface->pixels[index] = find_color(color);
}

static GifPixelType find_color(uint16_t color)
{
    switch (color) {
    case BLACK:
        return 0;
    case WHITE:
        return 1;
    case GRAY:
        return 2;
    default:
        panic();
        return 0;
    }
}
