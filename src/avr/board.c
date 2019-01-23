#include "board.h"
#include "dispif.h"
#include "pin.h"
#include "spi.h"
#include "uart.h"
#include "uart_hw.h"

#ifdef SW_SPI
#include "spi_sw.h"
#else
#include "spi_hw.h"
#endif

struct board_t board;

static struct dispif_t dispif;
static struct spi_t spi;
static struct pin_t led;
#ifdef SW_SPI
static struct pin_t clk;
static struct pin_t sdi;
#endif
static struct pin_t rs;
static struct pin_t rst;
static struct pin_t cs;
static struct uart_t uart;

void board_init(void)
{
    led = pin_init(PIN_PORT_C, 0, PIN_DIR_OUTPUT);
#ifdef SW_SPI
    clk = pin_init(PIN_PORT_B, 5, PIN_DIR_OUTPUT); // SCK
    sdi = pin_init(PIN_PORT_B, 3, PIN_DIR_OUTPUT); // MOSI
#endif
    rs  = pin_init(PIN_PORT_C, 3, PIN_DIR_OUTPUT);
    rst = pin_init(PIN_PORT_C, 4, PIN_DIR_OUTPUT);
    cs  = pin_init(PIN_PORT_C, 5, PIN_DIR_OUTPUT);

#ifdef SW_SPI
    spi_sw_init(&spi, &sdi, &clk);
#else
    spi_hw_init(&spi);
#endif
    dispif_init(&dispif, &spi, &rs, &cs, &rst, &led);
    uart_hw_init(&uart);

    board.dispif = &dispif;
    board.led = &led;
    board.uart = &uart;
}
