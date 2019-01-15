#include "dispif.h"
#include "spi.h"

void dispif_init(struct dispif_t *dispif, struct spi_t *spi, struct pin_t *rs, struct pin_t *cs, struct pin_t *rst, struct pin_t *led) {
    dispif->rs = rs;
    dispif->cs = cs;
    dispif->rst = rst;
    dispif->led = led;
    dispif->spi = spi;
    pin_write(dispif->cs, false);
}

static void write(struct dispif_t *dispif, uint16_t x) {
    spi_write(dispif->spi, x);
}

void dispif_write_command(struct dispif_t *dispif, uint16_t command) {
    pin_write(dispif->rs, false);
    write(dispif, command);
}

void dispif_write_data(struct dispif_t *dispif, uint16_t data) {
    pin_write(dispif->rs, true);
    write(dispif, data);
}

void dispif_write_register(struct dispif_t *dispif, uint16_t command, uint16_t data) {
    dispif_write_command(dispif, command);
    dispif_write_data(dispif, data);
}

void dispif_set_backlight(struct dispif_t *dispif, bool on) {
    pin_write(dispif->led, on);
}

void dispif_set_reset(struct dispif_t *dispif, bool on) {
    pin_write(dispif->rst, on);
}
