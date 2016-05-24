#include <stdlib.h>
#include <string.h>

#include "screen.h"

struct Screen {
    indexmap indices;
    FontInfo *font;
    int cursor_enabled;
    int cursor_x;
    int cursor_y;
    int dirty[];
};

static int screen_is_dirty(screen scr, int x, int y);
static void screen_set_dirty(screen scr, int x, int y, int dirty);
static void screen_clear_dirty(screen scr);

screen screen_init(indexmap indices, FontInfo *font) {
    const size_t size = indexmap_width(indices) * indexmap_height(indices);
    screen scr = malloc(sizeof(struct Screen) + size * sizeof(int));
    memset(scr, 0, sizeof(struct Screen) + size * sizeof(int));
    scr->indices = indices;
    scr->font = font;
    return scr;
}

void screen_set(screen scr, int x, int y, int val) {
    if (val != indexmap_get(scr->indices, x, y)) {
        indexmap_set(scr->indices, x, y, val);
        scr->dirty[y * indexmap_width(scr->indices) + x] = 1;
    }
}

int screen_get(screen scr, int x, int y) {
    scr->dirty[y * indexmap_width(scr->indices) + x] = 0;
    return indexmap_get(scr->indices, x, y);
}

void screen_enable_cursor(screen scr) {
    screen_set_dirty(scr, scr->cursor_x, scr->cursor_y, 1);
    scr->cursor_enabled = 1;
}

void screen_disable_cursor(screen scr) {
    screen_set_dirty(scr, scr->cursor_x, scr->cursor_y, 1);
    scr->cursor_enabled = 0;
}

void screen_move_cursor(screen scr, int x, int y) {
    if (scr->cursor_enabled) {
        screen_set_dirty(scr, scr->cursor_x, scr->cursor_y, 1);
        screen_set_dirty(scr, x, y, 1);
    }
    scr->cursor_x = x;
    scr->cursor_y = y;
}

static int screen_is_dirty(screen scr, int x, int y) {
    return scr->dirty[y * indexmap_width(scr->indices) + x];
}

static void screen_set_dirty(screen scr, int x, int y, int dirty) {
    scr->dirty[y * indexmap_width(scr->indices) + x] = dirty;
}

static void screen_clear_dirty(screen scr) {
    const size_t size = indexmap_width(scr->indices) * indexmap_height(scr->indices);
    memset(scr->dirty, 0, size * sizeof(int));
}

void screen_get_dirties(screen scr, DirtyIterator *dirties) {
    dirties->scr = scr;
    dirties->index = -1;
    dirties->x = 0;
    dirties->y = 0;
}

int screen_get_next_dirty(DirtyIterator *dirties) {
    const size_t size = indexmap_width(dirties->scr->indices) * indexmap_height(dirties->scr->indices);
    int i = 0;
    if (dirties->index > 0) {
        i = dirties->index;
    }
    for (; i < size; ++i) {
        if (dirties->scr->dirty[i]) {
            dirties->index = i;
            dirties->x = i % indexmap_width(dirties->scr->indices);
            dirties->y = i / indexmap_height(dirties->scr->indices);
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
                if (screen_is_dirty(scr, j, i)) {
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
