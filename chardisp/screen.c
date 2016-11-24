#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "screen.h"
#include "indexmap.h"

struct Screen {
    struct indexmap *indices;
    struct font *font;
    int cursor_enabled;
    int cursor_x;
    int cursor_y;
};

static void screen_clear_dirty(screen scr);

screen screen_init(struct indexmap *indices, struct font *font) {
    screen scr = malloc(sizeof(struct Screen));
    memset(scr, 0, sizeof(struct Screen));
    scr->indices = indices;
    scr->font = font;
    return scr;
}

void screen_set(screen scr, int x, int y, int val) {
    if (val != indexmap_get(scr->indices, x, y)) {
        indexmap_set(scr->indices, x, y, val);
        indexmap_set_dirty(scr->indices, x, y);
    }
}

int screen_get(screen scr, int x, int y) {
    indexmap_clear_dirty(scr->indices, x, y);
    return indexmap_get(scr->indices, x, y);
}

void screen_enable_cursor(screen scr) {
    indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
    scr->cursor_enabled = 1;
}

void screen_disable_cursor(screen scr) {
    indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
    scr->cursor_enabled = 0;
}

void screen_move_cursor(screen scr, int x, int y) {
    if (scr->cursor_enabled) {
        indexmap_set_dirty(scr->indices, scr->cursor_x, scr->cursor_y);
        indexmap_set_dirty(scr->indices, x, y);
    }
    scr->cursor_x = x;
    scr->cursor_y = y;
}

static void screen_clear_dirty(screen scr) {
    for (size_t x = 0; x < indexmap_width(scr->indices); x++) {
        for (size_t y = 0; y < indexmap_height(scr->indices); y++) {
            indexmap_clear_dirty(scr->indices, x, y);
        }
    }
}

void screen_get_dirties(screen scr, DirtyIterator *dirties) {
    dirties->scr = scr;
    dirties->index = -1;
    dirties->x = 0;
    dirties->y = 0;
}

int screen_get_next_dirty(DirtyIterator *dirties) {
    struct indexmap *indices = dirties->scr->indices;
    const size_t size = indexmap_width(indices) * indexmap_height(indices);
    int i = 0;
    if (dirties->index > 0) {
        i = dirties->index;
    }
    for (; i < size; ++i) {
        size_t x = i % indexmap_width(indices);
        size_t y = i / indexmap_height(indices);
        if (indexmap_is_dirty(indices, x, y)) {
            dirties->index = i;
            dirties->x = x;
            dirties->y = y;
            return 1;
        }
    }
    return 0;
}

void screen_print(screen scr) {
    int height = indexmap_height(scr->indices);
    int width = indexmap_width(scr->indices);
    char line[width * scr->font->width + 1];
    for (int i = 0; i < height; ++i) {
        for (int k = 0; k < scr->font->height; ++k) {
            for (int j = 0; j < width; ++j) {
                font_sprint_char(scr->font, &line[j * scr->font->width], indexmap_get(scr->indices, j, i), k);
                if (scr->cursor_enabled && scr->cursor_x == j && scr->cursor_y == i && k == scr->font->height - 1) {
                    memset(&line[j * scr->font->width], '$', scr->font->width);
                }
            }
            line[width * scr->font->width] = '\0';
            puts(line);
        }
    }
    screen_clear_dirty(scr);
}

void screen_print_dirty(screen scr) {
    int height = indexmap_height(scr->indices);
    int width = indexmap_width(scr->indices);
    char line[width * scr->font->width + 1];
    for (int i = 0; i < height; ++i) {
        for (int k = 0; k < scr->font->height; ++k) {
            for (int j = 0; j < width; ++j) {
                if (indexmap_is_dirty(scr->indices, j, i)) {
                    font_sprint_char(scr->font, &line[j * scr->font->width], indexmap_get(scr->indices, j, i), k);
                    if (scr->cursor_enabled && scr->cursor_x == j && scr->cursor_y == i && k == scr->font->height - 1) {
                        memset(&line[j * scr->font->width], '$', scr->font->width);
                    }
                } else {
                    memset(&line[j * scr->font->width], ' ', scr->font->width);
                }
            }
            line[width * scr->font->width] = '\0';
            puts(line);
        }
    }
    screen_clear_dirty(scr);
}
