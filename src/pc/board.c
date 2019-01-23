#include "board.h"
#include "dispif.h"
#include "pin.h"
#include "spi.h"
#include "spi_stdio.h"
#include "uart.h"
#include "uart_stdio.h"

struct board_t board;

static struct dispif_t dispif;
static struct spi_t spi;
static struct pin_t led;
static struct pin_t rs;
static struct pin_t rst;
static struct pin_t cs;
static struct uart_t uart;

void board_init(void)
{
    led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);
    rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);
    rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);
    cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);

    spi_stdio_init(&spi);
    dispif_init(&dispif, &spi, &rs, &cs, &rst, &led);
    uart_stdio_init(&uart);

    board.dispif = &dispif;
    board.led = &led;
    board.uart = &uart;
}
