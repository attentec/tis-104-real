#include <stdio.h>

#include "board.h"
#include "delay.h"
#include "panic.h"
#include "pin.h"
#include "uart.h"

void panic_handler(const char* file, int line) {
    uart_init();
    char buffer[200];
    sprintf(buffer, "%s:%d", file, line);
    write_line((uint8_t *) buffer);
    for (;;) {
        pin_write(board.led, false);
        delay_ms(100);
        pin_write(board.led, true);
        delay_ms(900);
    }
}
