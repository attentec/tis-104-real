#pragma once

#include <inttypes.h>

struct font
{
	uint8_t* data;
	uint8_t width;
	uint8_t height;
	uint8_t offset;
	uint8_t numchars;
};

void font_init(struct font *font, uint8_t *data);
uint8_t font_char_column(struct font *font, uint8_t charno, uint8_t column);
void font_sprint_char(struct font *font, char *buf, char ch, int line);
