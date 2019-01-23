#pragma once

#include <stdint.h>

struct uart_t {
    uint8_t dummy;
};

void uart_init(struct uart_t *uart);
void uart_write_char(struct uart_t *uart, uint8_t c);
uint8_t uart_read_char(struct uart_t *uart);
void uart_write_line(struct uart_t *uart, uint8_t *s);
