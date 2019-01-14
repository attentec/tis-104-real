#pragma once

#include <stdint.h>

struct spi_t {
    uint8_t dummy;  
};

void spi_init(struct spi_t *spi);
