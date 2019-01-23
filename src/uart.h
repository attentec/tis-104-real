#pragma once

#include <stdint.h>

void uart_init(void);
void write_char(uint8_t c);
uint8_t read_char(void);
void write_line(uint8_t *s);
