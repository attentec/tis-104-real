#include "platform.h"
#include "runner.h"
#include "uart.h"

int main(int argc, char **argv)
{
    runner_main(ORIENTATION_RIBBON_BOTTOM, 1, 1, argc, argv);
    return 0;
}

void app_init(void)
{
    char c = (char) uart_read_char(platform.uart);
    if ((c >= 'A' && c<= 'M') || (c >= 'a' && c<= 'm')) {
        c += 13;
    } else if ((c >= 'N' && c<= 'Z') || (c >= 'n' && c<= 'z')) {
        c -= 13;
    }
    uart_write_char(platform.uart, (uint8_t) c);
}

bool app_loop(void)
{
    return true;
}