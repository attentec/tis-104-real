#include "board.h"
#include "uart.h"

int main(void)
{
    board_init();
    for (;;) {
        char c = (char) uart_read_char(board.uart);
        if ((c >= 'A' && c<= 'M') || (c >= 'a' && c<= 'm')) {
            c += 13;
        } else if ((c >= 'N' && c<= 'Z') || (c >= 'n' && c<= 'z')) {
            c -= 13;
        }
        uart_write_char(board.uart, (uint8_t) c);
    }
    return 0;
}
