#pragma once

#include "pin.h"

struct spi_t {
    pin_t sdi;
    pin_t clk;
};

void spi_init(struct spi_t *spi, pin_t sdi, pin_t clk);
void spi_write(struct spi_t *spi, uint8_t byte);
