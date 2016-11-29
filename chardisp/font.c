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
