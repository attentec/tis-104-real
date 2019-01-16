#pragma once

#define DISPLAY_WIDTH (176)
#define DISPLAY_HEIGHT (220)

#define RGB888_TO_RGB565(x) ((((x >> 19u) & 0x1Fu) << 11u) | \
                             (((x >> 10u) & 0x3Fu) <<  5u) | \
                             (((x >>  3u) & 0x1Fu) <<  0u))

struct dispif_t;

struct display_t {
    struct dispif_t *dispif;
};

void display_init(struct display_t *display, struct dispif_t *dispif);
void display_activate(struct display_t *display);
void display_set_window(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void display_write_pixel(struct display_t *display, uint16_t color);