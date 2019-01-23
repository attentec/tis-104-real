#include <stdio.h>

#include "panic.h"
#include "uart.h"

void uart_init(void)
{
}

void write_char(uint8_t c)
{
    int ret = putchar((char) c);
    if (ret == EOF) {
        panic();
    }
}

uint8_t read_char(void)
{
    int ret = getchar();
    if (ret == EOF) {
        panic();
    }
    return (uint8_t) ret;
}

void write_line(uint8_t *s)
{
    for (uint8_t *p = s; *p; p++) {
        write_char(*p);
    }
    write_char('\r');
    write_char('\n');
}
