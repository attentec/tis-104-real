#include <stdio.h>

#include "board.h"
#include "delay.h"
#include "panic.h"
#include "pin.h"
#include "uart.h"

void panic_handler(const char* file, int line) {
    struct uart_t *uart = board.uart;
    char buffer[200];
    sprintf(buffer, "%s:%d", file, line);
    uart_write_line(uart, (uint8_t *) buffer);
    for (;;) {
        pin_write(board.led, false);
        delay_ms(100);
        pin_write(board.led, true);
        delay_ms(900);
    }
}
