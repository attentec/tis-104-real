#include "font.h"
#include "mem.h"

#define FONT_HEADER_SIZE (4u) // 1: pixel width of 1 font character, 2: pixel height

void font_init(struct font *font, uint8_t *data) {
    font->data = data;
    font->width = mem_read_byte(&data[0]);
    font->height = mem_read_byte(&data[1]);
    font->offset = mem_read_byte(&data[2]);
    font->numchars = mem_read_byte(&data[3]);
}

uint8_t font_read_column(struct font *font, uint8_t charno, uint8_t column) {
    uint16_t char_width = font->width + 1u; // bytes used by each character
    uint16_t char_offset = charno - font->offset; // shift charno into font subrange
    uint16_t char_index = (char_width * char_offset) + FONT_HEADER_SIZE;
    return mem_read_byte(&font->data[char_index + column + 1u]);
}

void font_sprint_char(struct font *font, char *buf, char ch, int line) {
    uint8_t char_data, char_width;
    uint8_t i, j;

    for (i = 0; i < font->width; i++) {  // each font "column"
        char_data = font_read_column(font, ch, i);

        if (line >= font->height) {
            break;  // No need to process excess bits
        }
        if (char_data & (1 << line)) {
            buf[i] = '#';
        } else {
            buf[i] = ' ';
        }
    }
}
