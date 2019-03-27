#include "platform.h"
#include "runner.h"
#include "uart.h"

int main(void)
{
    runner_main(ORIENTATION_DEFAULT);
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