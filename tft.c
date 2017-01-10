#include <stdbool.h>
#include <string.h>

#include "delay.h"
#include "disp.h"
#include "panic.h"
#include "pin.h"
#include "tft.h"

static void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

static struct tft tft;
static struct disp_t _disp;
static struct disp_t *disp = &_disp;

// Helper functions
static void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR1, x1);
    disp_write_register(disp, ILI9225_HORIZONTAL_WINDOW_ADDR2, x0);

    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR1, y1);
    disp_write_register(disp, ILI9225_VERTICAL_WINDOW_ADDR2, y0);

    disp_write_register(disp, ILI9225_RAM_ADDR_SET1, x0);
    disp_write_register(disp, ILI9225_RAM_ADDR_SET2, y0);

    disp_write_command(disp, 0x22);
}

// Constructor when using software SPI.  All output pins are configurable.
void tft_swspi(uint8_t rst, uint8_t rs, uint8_t cs, uint8_t sdi, uint8_t clk, uint8_t led, struct screen *scr) {
    disp_init(disp, rs, cs, rst, led, sdi, clk);
    tft.scr = scr;
    tft.maxX = ILI9225_LCD_WIDTH;
    tft.maxY = ILI9225_LCD_HEIGHT;
}

void tft_begin() {
    // Turn on backlight
    disp_set_backlight(disp, true);

    // Initialization Code
    disp_set_reset(disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    delay_ms(1);
    disp_set_reset(disp, false); // Pull the reset pin low to reset ILI9225
    delay_ms(10);
    disp_set_reset(disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    delay_ms(50);

    /* Start Initial Sequence */
    /* Set SS bit and direction output from S528 to S1 */
    disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(disp, ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    disp_write_register(disp, ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    disp_write_register(disp, ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    delay_ms(40);

    // Power-on sequence
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(disp, ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    disp_write_register(disp, ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    disp_write_register(disp, ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    delay_ms(10);
    disp_write_register(disp, ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    delay_ms(50);

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
    delay_ms(50);
    disp_write_register(disp, ILI9225_DISP_CTRL1, 0x1017);

    tft_setBacklight(true);

    // Initialize variables
    tft_setBackgroundColor(COLOR_BLACK);
    tft_setForegroundColor(COLOR_WHITE);

    tft_clear();
}

void tft_clear() {
    tft_fillRectangle(0, 0, tft.maxX - 1, tft.maxY - 1, COLOR_BLACK);
    delay_ms(10);
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
        delay_ms(50);
        disp_write_register(disp, ILI9225_DISP_CTRL1, 0x1017);
        delay_ms(200);
    } else {
        disp_write_register(disp, 0x00ff, 0x0000);
        disp_write_register(disp, ILI9225_DISP_CTRL1, 0x0000);
        delay_ms(50);
        disp_write_register(disp, ILI9225_POWER_CTRL1, 0x0003);
        delay_ms(200);
    }
}

void tft_fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    setWindow(x1, y1, x2, y2);

    for(uint16_t t=(y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        disp_write_data(disp, color);
    }
}

void tft_setBackgroundColor(uint16_t color) {
    tft.bgColor = color;
}

void tft_setForegroundColor(uint16_t color) {
    tft.fgColor = color;
}

void tft_setFont(struct font *font) {
    tft.cfont = font;
}

void tft_drawText(uint8_t x, uint8_t y, char *s) {
    uint8_t currx = x;

    // Print every character in string
    for (uint8_t k = 0; k < strlen(s); k++) {
        screen_set(tft.scr, currx, y, s[k]);
        currx += 1;
    }
}

void tft_render() {
    DirtyIterator dirties;
    screen_get_dirties(tft.scr, &dirties);
    while (screen_get_next_dirty(&dirties)) {
        tft_drawChar(dirties.x, dirties.y, screen_get(tft.scr, dirties.x, dirties.y));
    }
}

uint16_t tft_drawChar(uint8_t x, uint8_t y, char ch) {
    uint16_t pixel_x = tft.maxX - (uint16_t)(y + 1) * tft.cfont->height;
    uint16_t pixel_y = (uint16_t)x * tft.cfont->width;
    setWindow(pixel_x, pixel_y, pixel_x + tft.cfont->height - 1, pixel_y + tft.cfont->width - 1);
    uint16_t charPixels[6 * 8] = {0};
    for (uint8_t i = 0; i < tft.cfont->width; i++) {  // each font "column"
        uint8_t charData = font_read_column(tft.cfont, ch, i);

        // Process every row in font character
        for (uint8_t k = 0; k < tft.cfont->height; k++) {
            uint8_t j = tft.cfont->height - k - 1;
            if (charData & 1) {
                charPixels[i * 8 + j] = tft.fgColor;
            } else {
                charPixels[i * 8 + j] = tft.bgColor;
            }
            charData >>= 1;
        }
    }
    for (uint8_t i = 0; i < (6 * 8); i++) {
        disp_write_data(disp, charPixels[i]);
    }
    return tft.cfont->width;
}
