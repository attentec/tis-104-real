#include "disp.h"
#include "spi.h"

void disp_init(struct disp_t *disp, struct spi_t *spi, pin_t rs, pin_t cs, pin_t rst, pin_t led) {
    disp->rs = rs;
    disp->cs = cs;
    disp->rst = rst;
    disp->led = led;
    disp->spi = spi;
}

static void write(struct disp_t *disp, uint16_t x) {
    pin_write(disp->cs, false);
    spi_write(disp->spi, x);
    pin_write(disp->cs, true);
}

void disp_write_command(struct disp_t *disp, uint16_t command) {
    pin_write(disp->rs, false);
    write(disp, command);
}

void disp_write_data(struct disp_t *disp, uint16_t data) {
    pin_write(disp->rs, true);
    write(disp, data);
}

void disp_write_register(struct disp_t *disp, uint16_t command, uint16_t data) {
    disp_write_command(disp, command);
    disp_write_data(disp, data);
}

void disp_set_backlight(struct disp_t *disp, bool on) {
    pin_write(disp->led, on);
}

void disp_set_reset(struct disp_t *disp, bool on) {
    pin_write(disp->rst, on);
}
