#include <unistd.h>

#include "delay.h"

void delay_ms(uint16_t ms) {
    (void)sleep(ms / 1000);
}
