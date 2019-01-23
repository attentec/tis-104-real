#pragma once

#include "uart.h"

struct uart_t {
    uint8_t dummy;
};

void uart_stdio_init(struct uart_t *uart);
