#pragma once

#include <stdint.h>

struct spi_t;

void spi_write(struct spi_t *spi, uint16_t word);
