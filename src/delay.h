#pragma once

#ifdef __AVR__

#include <util/delay.h>

#define delay_ms _delay_ms
#define delay_us _delay_us

#else

#include <stdint.h>

void delay_ms(uint16_t ms);
void delay_us(uint16_t us);

#endif
