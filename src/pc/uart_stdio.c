#include <stdio.h>

#include "panic.h"
#include "uart.h"
#include "uart_stdio.h"

void uart_stdio_init(struct uart_t *uart)
{
    (void) uart;
}

void uart_write_char(struct uart_t *uart, uint8_t c)
{
    (void) uart;
    int ret = putchar((char) c);
    if (ret == EOF) {
        panic();
    }
}

uint8_t uart_read_char(struct uart_t *uart)
{
    (void) uart;
    int ret = getchar();
    if (ret == EOF) {
        panic();
    }
    return (uint8_t) ret;
}

void uart_write_line(struct uart_t *uart, uint8_t *s)
{
    (void) uart;
    for (uint8_t *p = s; *p; p++) {
        uart_write_char(uart, *p);
    }
    uart_write_char(uart, '\r');
    uart_write_char(uart, '\n');
}
