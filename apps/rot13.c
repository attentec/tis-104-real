#include "uart.h"

int main(void)
{
    struct uart_t uart;
    uart_init(&uart);
    for (;;) {
        char c = (char) uart_read_char(&uart);
        if ((c >= 'A' && c<= 'M') || (c >= 'a' && c<= 'm')) {
            c += 13;
        } else if ((c >= 'N' && c<= 'Z') || (c >= 'n' && c<= 'z')) {
            c -= 13;
        }
        uart_write_char(&uart, (uint8_t) c);
    }
    return 0;
}
