#include "uart.h"

int main(void)
{
    uart_init();
    for (;;) {
        char c = (char) read_char();
        if ((c >= 'A' && c<= 'M') || (c >= 'a' && c<= 'm')) {
            c += 13;
        } else if ((c >= 'N' && c<= 'Z') || (c >= 'n' && c<= 'z')) {
            c -= 13;
        }
        write_char((uint8_t) c);
    }
    return 0;
}
