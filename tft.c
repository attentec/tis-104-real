#include <util/delay.h>
#include <string.h>
#include <stdbool.h>

#include "tft.h"
#include "pin.h"
#include "panic.h"

static void swap(uint16_t *a, uint16_t *b);
static void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
static void orientCoordinates(uint16_t *x1, uint16_t *y1);
static uint16_t joinColor(uint8_t red8, uint8_t green8, uint8_t blue8);
static void splitColor(uint16_t rgb, uint8_t *red, uint8_t *green, uint8_t *blue);
static void drawPixel(uint16_t x1, uint16_t y1, uint16_t color);
static void pinMode(uint8_t pin, enum Direction dir);
static uint8_t bitRead(uint8_t byte, uint8_t index);

static struct tft tft;
static struct disp_t _disp;
static struct disp_t *disp = &_disp;

// Helper functions
static void drawPixel(uint16_t x1, uint16_t y1, uint16_t color) {
    if ((x1 < 0) || (x1 >= tft.maxX) || (y1 < 0) || (y1 >= tft.maxY)) {
        return;
    }

    disp_write_data(disp, color);
}

static uint16_t joinColor(uint8_t red8, uint8_t green8, uint8_t blue8) {
    // rgb16 = red5 green6 blue5
    return (red8 >> 3) << 11 | (green8 >> 2) << 5 | (blue8 >> 3);
}

static void splitColor(uint16_t rgb, uint8_t *red, uint8_t *green, uint8_t *blue) {
    // rgb16 = red5 green6 blue5
    *red   = (rgb & 0b1111100000000000) >> 11 << 3;
    *green = (rgb & 0b0000011111100000) >>  5 << 2;
    *blue  = (rgb & 0b0000000000011111)       << 3;
}

static void orientCoordinates(uint16_t *x1, uint16_t *y1) {
    switch (tft.orientation) {
        case 0:  // ok
            break;
        case 1: // ok
            *y1 = tft.maxY - *y1 - 1;
            swap(x1, y1);
            break;
        case 2: // ok
            *x1 = tft.maxX - *x1 - 1;
            *y1 = tft.maxY - *y1 - 1;
            break;
        case 3: // ok
            *x1 = tft.maxX - *x1 - 1;
            swap(x1, y1);
            break;
    }
}

static void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR1, x1);
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR2, x0);

    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR1, y1);
    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR2, y0);

    disp_write_register(disp, ILI9225_RAM_ADDR_SET1, x0);
    disp_write_register(disp, ILI9225_RAM_ADDR_SET2, y0);

    disp_write_command(disp, 0x22);
}

static void swap(uint16_t *a, uint16_t *b) {
    uint16_t w = *a;
    *a = *b;
    *b = w;
}

// Arduino-like functions
static void pinMode(uint8_t pin, enum Direction dir) {
    if (pin < 8) {
        if (dir == INPUT) {
            DDRD &= ~(1 << pin);
        } else {
            DDRD |= (1 << pin);
        }
    } else {
        uint8_t index = pin - 8;
        if (dir == INPUT) {
            DDRB &= ~(1 << index);
        } else {
            DDRB |= (1 << index);
        }
    }
}

static uint8_t bitRead(uint8_t byte, uint8_t index) {
    return byte & (1 << index);
}

// Constructor when using software SPI.  All output pins are configurable.
void tft_swspi(uint8_t rst, uint8_t rs, uint8_t cs, uint8_t sdi, uint8_t clk, uint8_t led, screen scr) {
    disp_init(disp, rs, cs, rst, led, sdi, clk);
    tft.scr = scr;
}

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
void tft_hwspi(uint8_t rst, uint8_t rs, uint8_t cs, uint8_t led) {
    panic();
    disp->rst  = rst;
    disp->rs   = rs;
    disp->cs   = cs;
    // disp->sdi  = 0;
    // disp->clk = 0;
    disp->led  = led;
}

void tft_begin() {
    // Turn on backlight
    disp_set_backlight(disp, true);

    // Initialization Code
    disp_set_reset(disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    _delay_ms(1);
    disp_set_reset(disp, false); // Pull the reset pin low to reset ILI9225
    _delay_ms(10);
    disp_set_reset(disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    _delay_ms(50);

    /* Start Initial Sequence */
    /* Set SS bit and direction output from S528 to S1 */
    disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(disp, ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    disp_write_register(disp, ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    disp_write_register(disp, ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    _delay_ms(40);

    // Power-on sequence
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(disp, ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    disp_write_register(disp, ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    disp_write_register(disp, ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    _delay_ms(10);
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    _delay_ms(50);

    disp_write_register(disp, ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    disp_write_register(disp, ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    disp_write_register(disp, ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
    disp_write_register(disp, ILI9225_DISP_CTRL1, 0x0000); // Display off
    disp_write_register(disp, ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
    disp_write_register(disp, ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
    disp_write_register(disp, ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    disp_write_register(disp, ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
    disp_write_register(disp, ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
    disp_write_register(disp, ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
    disp_write_register(disp, ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

    /* Set GRAM area */
    disp_write_register(disp, ILI9225_GATE_SCAN_CTRL, 0x0000);
    disp_write_register(disp, ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    disp_write_register(disp, ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    disp_write_register(disp, ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    disp_write_register(disp, ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    disp_write_register(disp, ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    /* Set GAMMA curve */
    disp_write_register(disp, ILI9225_GAMMA_CTRL1, 0x0000);
    disp_write_register(disp, ILI9225_GAMMA_CTRL2, 0x0808);
    disp_write_register(disp, ILI9225_GAMMA_CTRL3, 0x080A);
    disp_write_register(disp, ILI9225_GAMMA_CTRL4, 0x000A);
    disp_write_register(disp, ILI9225_GAMMA_CTRL5, 0x0A08);
    disp_write_register(disp, ILI9225_GAMMA_CTRL6, 0x0808);
    disp_write_register(disp, ILI9225_GAMMA_CTRL7, 0x0000);
    disp_write_register(disp, ILI9225_GAMMA_CTRL8, 0x0A00);
    disp_write_register(disp, ILI9225_GAMMA_CTRL9, 0x0710);
    disp_write_register(disp, ILI9225_GAMMA_CTRL10, 0x0710);

    disp_write_register(disp, ILI9225_DISP_CTRL1, 0x0012);
    _delay_ms(50);
    disp_write_register(disp, ILI9225_DISP_CTRL1, 0x1017);

    tft_setBacklight(true);
    tft_setOrientation(0);

    // Initialize variables
    tft_setBackgroundColor(COLOR_BLACK);

    tft_clear();
}

void tft_clear() {
    uint8_t old = tft.orientation;
    tft_setOrientation(0);
    tft_fillRectangle(0, 0, tft.maxX - 1, tft.maxY - 1, COLOR_BLACK);
    tft_setOrientation(old);
    _delay_ms(10);
}

void tft_invert(bool flag) {
    disp_write_command(disp, flag ? ILI9225C_INVON : ILI9225C_INVOFF);
}

void tft_setBacklight(bool flag) {
    disp_set_backlight(disp, flag);
}

void tft_setDisplay(bool flag) {
    if (flag) {
        disp_write_register(disp, 0x00ff, 0x0000);
        disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0000);
        _delay_ms(50);
        disp_write_register(disp, ILI9225_DISP_CTRL1, 0x1017);
        _delay_ms(200);
    } else {
        disp_write_register(disp, 0x00ff, 0x0000);
        disp_write_register(disp, ILI9225_DISP_CTRL1, 0x0000);
        _delay_ms(50);
        disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0003);
        _delay_ms(200);
    }
}

void tft_setOrientation(uint8_t orientation) {
    tft.orientation = orientation % 4;

    switch (tft.orientation) {
        case 0:
            tft.maxX = ILI9225_LCD_WIDTH;
            tft.maxY = ILI9225_LCD_HEIGHT;
            break;
        case 1:
            tft.maxX = ILI9225_LCD_HEIGHT;
            tft.maxY = ILI9225_LCD_WIDTH;
            break;
        case 2:
            tft.maxX = ILI9225_LCD_WIDTH;
            tft.maxY = ILI9225_LCD_HEIGHT;
            break;
        case 3:
            tft.maxX = ILI9225_LCD_HEIGHT;
            tft.maxY = ILI9225_LCD_WIDTH;
            break;
    }
}

uint8_t tft_getOrientation() {
    return tft.orientation;
}

void tft_drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    tft_drawLine(x1, y1, x1, y2, color);
    tft_drawLine(x1, y1, x2, y1, color);
    tft_drawLine(x1, y2, x2, y2, color);
    tft_drawLine(x2, y1, x2, y2, color);
}

void tft_fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    setWindow(x1, y1, x2, y2);

    for(uint16_t t=(y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        disp_write_data(disp, color);
    }
}

void tft_drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    // Classic Bresenham algorithm
    int16_t steep = abs(y2 - y1) > abs(x2 - x1);
    int16_t dx, dy;

    if (steep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }

    if (x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    dx = x2 - x1;
    dy = abs(y2 - y1);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y1 < y2) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x1 <= x2; x1++) {
        if (steep) {
            drawPixel(y1, x1, color);
        } else {
            drawPixel(x1, y1, color);
        }

        err -= dy;
        if (err < 0) {
            y1 += ystep;
            err += dx;
        }
    }
}

uint16_t tft_maxX() {
    return tft.maxX;
}

uint16_t tft_maxY() {
    return tft.maxY;
}

void tft_setBackgroundColor(uint16_t color) {
    tft.bgColor = color;
}

void tft_setFont(struct font *font) {
    tft.cfont = font;
}

void tft_drawText(uint16_t x, uint16_t y, char *s, uint16_t color) {
    uint16_t currx = x;

    // Print every character in string
    for (uint8_t k = 0; k < strlen(s); k++) {
        screen_set(tft.scr,
                   currx / tft.cfont->width,
                   y / tft.cfont->height,
                   s[k] - tft.cfont->offset);
        currx += tft.cfont->width;
    }
}

void tft_render() {
    DirtyIterator dirties;
    screen_get_dirties(tft.scr, &dirties);
    while (screen_get_next_dirty(&dirties)) {
        tft_drawChar(dirties.x * tft.cfont->width,
                     dirties.y * tft.cfont->height,
                     screen_get(tft.scr, dirties.x, dirties.y),
                     COLOR_WHITE);
    }
}

uint16_t tft_drawChar(uint16_t x, uint16_t y, uint16_t ch, uint16_t color) {
    setWindow(x, y, x + tft.cfont->width - 1, y + tft.cfont->height - 1);
    uint16_t charPixels[6 * 8] = {0};
    for (uint8_t i = 0; i < tft.cfont->width; i++) {  // each font "column"
        uint8_t charData = font_read_column(tft.cfont, ch, i);

        // Process every row in font character
        for (uint8_t k = 0; k < tft.cfont->height; k++) {
            if (charData & 1) {
                charPixels[k * 6 + i] = color;
            } else {
                charPixels[k * 6 + i] = tft.bgColor;
            }
            charData >>= 1;
        }
    }
    for (uint8_t i = 0; i < 6 * 8; i++) {
        disp_write_data(disp, charPixels[i]);
    }
    return tft.cfont->width;
}
