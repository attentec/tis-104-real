#include <stdio.h>

#include "delay.h"
#include "panic.h"
#include "pin.h"
#include "platform.h"
#include "uart.h"

void panic_handler(const char* file, int line) {
    struct uart_t *uart = platform.uart;
    char buffer[200];
    sprintf(buffer, "%s:%d", file, line);
    uart_write_line(uart, (uint8_t *) buffer);
    for (;;) {
        pin_write(platform.led, false);
        delay_ms(100);
        pin_write(platform.led, true);
        delay_ms(900);
    }
}
