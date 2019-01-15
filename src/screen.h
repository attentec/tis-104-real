#pragma once

#include <stdbool.h>
#include <stdint.h>

struct indexmap_t;

struct screen_t {
    struct indexmap_t *indices;
    bool cursor_enabled;
    uint8_t cursor_x;
    uint8_t cursor_y;
};

void screen_init(struct screen_t *scr, struct indexmap_t *indices);
void screen_set(struct screen_t *scr, uint8_t x, uint8_t y, uint8_t val);
uint8_t screen_get(struct screen_t *scr, uint8_t x, uint8_t y);
void screen_enable_cursor(struct screen_t *scr);
void screen_disable_cursor(struct screen_t *scr);
void screen_move_cursor(struct screen_t *scr, uint8_t x, uint8_t y);
void screen_print(struct screen_t *scr);
void screen_print_dirty(struct screen_t *scr);

struct dirties_t {
    struct screen_t *scr;
    uint16_t index;
    uint8_t x;
    uint8_t y;
};

void screen_get_dirties(struct screen_t *scr, struct dirties_t *dirties);
uint8_t screen_get_next_dirty(struct dirties_t *dirties);
