#include <avr/io.h>
#include <stdio.h>

#include "delay.h"
#include "panic.h"
#include "uart.h"

void panic_handler(const char* file, int line) {
    uart_init();
    char buffer[200];
    sprintf(buffer, "%s:%d", file, line);
    write_line((uint8_t *) buffer);
    for (;;) {
        PORTC &= ~(1<<0);
        delay_ms(100);
        PORTC |= (1<<0);
        delay_ms(900);
    }
}
