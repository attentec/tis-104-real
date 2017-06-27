#pragma once

#include <stdbool.h>
#include <stdint.h>

struct indexmap;

struct screen {
    struct indexmap *indices;
    bool cursor_enabled;
    uint8_t cursor_x;
    uint8_t cursor_y;
};

void screen_init(struct screen *scr, struct indexmap *indices);
void screen_set(struct screen *scr, uint8_t x, uint8_t y, uint8_t val);
uint8_t screen_get(struct screen *scr, uint8_t x, uint8_t y);
void screen_enable_cursor(struct screen *scr);
void screen_disable_cursor(struct screen *scr);
void screen_move_cursor(struct screen *scr, uint8_t x, uint8_t y);
void screen_print(struct screen *scr);
void screen_print_dirty(struct screen *scr);

typedef struct {
    struct screen *scr;
    uint16_t index;
    uint8_t x;
    uint8_t y;
} DirtyIterator;
void screen_get_dirties(struct screen *scr, DirtyIterator *dirties);
uint8_t screen_get_next_dirty(DirtyIterator *dirties);
