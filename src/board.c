#include "board.h"
#include "dispif.h"
#include "pin.h"
#include "spi.h"
#include "spi_hw.h"

static struct dispif_t dispif;
static struct spi_t spi;
static struct pin_t led;
//static struct pin_t clk;
//static struct pin_t sdi;
static struct pin_t rs;
static struct pin_t rst;
static struct pin_t cs;

void board_init(struct board_t *board)
{
    // Display module plugged directly into Arduino headers
                                                      // Arduino pin
    led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);    // A0
    //clk = pin_init(PIN_PORT_C, 1, PIN_DIR_OUTPUT);    // A1
    //sdi = pin_init(PIN_PORT_C, 2, PIN_DIR_OUTPUT);    // A2
    rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);    // A3
    rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);    // A4
    cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);    // A5

    spi_init(&spi);
    dispif_init(&dispif, &spi, &rs, &cs, &rst, &led);

    board->dispif = &dispif;
}
