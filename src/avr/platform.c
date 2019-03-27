#include "dispif.h"
#include "display.h"
#include "display_ili9225.h"
#include "panic.h"
#include "pin.h"
#include "platform.h"
#include "spi.h"
#include "uart.h"
#include "uart_hw.h"

#ifdef SW_SPI
#include "spi_sw.h"
#else
#include "spi_hw.h"
#endif

struct platform_t platform;

static struct dispif_t dispif;
static struct display_t display;
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

void platform_init(enum orientation_t orientation)
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
    display_ili9225_init(&display, &dispif, orientation);
    uart_hw_init(&uart);

    platform.display = &display;
    platform.led = &led;
    platform.uart = &uart;
}

void platform_init_multi(enum orientation_t orientation, int cols, int rows)
{
    if (cols != 1 || rows != 1) {
        panic();
    }
    platform_init(orientation);
}

struct display_t *platform_get_display(int col, int row)
{
    if (col != 0 || row != 0) {
        panic();
    }
    return platform.display;
}

void platform_begin(void)
{
    dispif_set_backlight(&dispif, true);
}

bool platform_loop(void)
{
    return true;
}

void platform_deinit(void)
{
}
