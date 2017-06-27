#include "indexmap.h"
#include "screen.h"

void screen_init(struct screen *scr, struct indexmap *indices) {
    scr->indices = indices;
    scr->cursor_enabled = false;
    scr->cursor_x = 0;
    scr->cursor_y = 0;
}

void screen_set(struct screen *scr, uint8_t x, uint8_t y, uint8_t val) {
    if (val != indexmap_get(scr->indices, x, y)) {
        indexmap_set(scr->indices, x, y, val);
        indexmap_set_dirty(scr->indices, x, y);
    }
}

uint8_t screen_get(struct screen *scr, uint8_t x, uint8_t y) {
    indexmap_clear_dirty(scr->indices, x, y);
    return indexmap_get(scr->indices, x, y);
}

void screen_enable_cursor(struct screen *scr) {
    indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
    scr->cursor_enabled = true;
}

void screen_disable_cursor(struct screen *scr) {
    indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
    scr->cursor_enabled = false;
}

void screen_move_cursor(struct screen *scr, uint8_t x, uint8_t y) {
    if (scr->cursor_enabled) {
        indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
        indexmap_set_dirty(scr->indices, x, y);
    }
    scr->cursor_x = x;
    scr->cursor_y = y;
}

void screen_get_dirties(struct screen *scr, DirtyIterator *dirties) {
    dirties->scr = scr;
    dirties->index = 0;
    dirties->x = 0;
    dirties->y = 0;
}

uint8_t screen_get_next_dirty(DirtyIterator *dirties) {
    struct indexmap *indices = dirties->scr->indices;
    const uint16_t size = indexmap_width(indices) * indexmap_height(indices);
    uint16_t i = 0;
    if (dirties->index > 0) {
        i = dirties->index;
    }
    for (; i < size; ++i) {
        uint8_t x = i % indexmap_width(indices);
        uint8_t y = i / indexmap_width(indices);
        if (indexmap_is_dirty(indices, x, y)) {
            dirties->index = i;
            dirties->x = x;
            dirties->y = y;
            return 1;
        }
    }
    return 0;
}
