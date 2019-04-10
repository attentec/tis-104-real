#include <stdint.h>
#include <time.h>

#include "delay.h"

void delay_ms(uint16_t ms) {
    struct timespec t = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000L) * 1000000L,
    };
    (void)nanosleep(&t, NULL);
}

void delay_us(uint16_t us) {
    struct timespec t = {
        .tv_sec = 0,
        .tv_nsec = us * 1000L,
    };
    (void)nanosleep(&t, NULL);
}
