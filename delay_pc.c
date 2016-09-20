#include <unistd.h>

#include "delay.h"

void delay_ms(uint16_t ms) {
    (void)sleep(ms / 1000);
}

void delay_us(uint16_t us) {
    (void)sleep(us / 1000000);
}
