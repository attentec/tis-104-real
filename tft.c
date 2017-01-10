#include <stdbool.h>
#include <string.h>

#include "delay.h"
#include "disp.h"
#include "panic.h"
#include "pin.h"
#include "tft.h"

static void setWindow(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

// Helper functions
static void setWindow(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    disp_write_register(tft->disp, ILI9225_HORIZONTAL_WINDOW_ADDR1, x1);
    disp_write_register(tft->disp, ILI9225_HORIZONTAL_WINDOW_ADDR2, x0);

    disp_write_register(tft->disp, ILI9225_VERTICAL_WINDOW_ADDR1, y1);
    disp_write_register(tft->disp, ILI9225_VERTICAL_WINDOW_ADDR2, y0);

    disp_write_register(tft->disp, ILI9225_RAM_ADDR_SET1, x0);
    disp_write_register(tft->disp, ILI9225_RAM_ADDR_SET2, y0);

    disp_write_command(tft->disp, 0x22);
}

void tft_init(struct tft_t *tft, struct disp_t *disp, struct screen *scr, struct font *font) {
    tft->cfont = font;
    tft->disp = disp;
    tft->scr = scr;
    tft->maxX = ILI9225_LCD_WIDTH;
    tft->maxY = ILI9225_LCD_HEIGHT;
}

void tft_begin(struct tft_t *tft) {
    // Turn on backlight
    disp_set_backlight(tft->disp, true);

    // Initialization Code
    disp_set_reset(tft->disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    delay_ms(1);
    disp_set_reset(tft->disp, false); // Pull the reset pin low to reset ILI9225
    delay_ms(10);
    disp_set_reset(tft->disp, true); // Pull the reset pin high to release the ILI9225C from the reset status
    delay_ms(50);

    /* Start Initial Sequence */
    /* Set SS bit and direction output from S528 to S1 */
    disp_write_register(tft->disp, ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
    disp_write_register(tft->disp, ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(tft->disp, ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
    disp_write_register(tft->disp, ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
    disp_write_register(tft->disp, ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
    delay_ms(40);

    // Power-on sequence
    disp_write_register(tft->disp, ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
    disp_write_register(tft->disp, ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
    disp_write_register(tft->disp, ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
    disp_write_register(tft->disp, ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
    disp_write_register(tft->disp, ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
    delay_ms(10);
    disp_write_register(tft->disp, ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
    delay_ms(50);

    disp_write_register(tft->disp, ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
    disp_write_register(tft->disp, ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
    disp_write_register(tft->disp, ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
    disp_write_register(tft->disp, ILI9225_DISP_CTRL1, 0x0000); // Display off
    disp_write_register(tft->disp, ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
    disp_write_register(tft->disp, ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
    disp_write_register(tft->disp, ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
    disp_write_register(tft->disp, ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
    disp_write_register(tft->disp, ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
    disp_write_register(tft->disp, ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
    disp_write_register(tft->disp, ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

    /* Set GRAM area */
    disp_write_register(tft->disp, ILI9225_GATE_SCAN_CTRL, 0x0000);
    disp_write_register(tft->disp, ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
    disp_write_register(tft->disp, ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
    disp_write_register(tft->disp, ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
    disp_write_register(tft->disp, ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
    disp_write_register(tft->disp, ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
    disp_write_register(tft->disp, ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
    disp_write_register(tft->disp, ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
    disp_write_register(tft->disp, ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
    disp_write_register(tft->disp, ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

    /* Set GAMMA curve */
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL1, 0x0000);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL2, 0x0808);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL3, 0x080A);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL4, 0x000A);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL5, 0x0A08);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL6, 0x0808);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL7, 0x0000);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL8, 0x0A00);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL9, 0x0710);
    disp_write_register(tft->disp, ILI9225_GAMMA_CTRL10, 0x0710);

    disp_write_register(tft->disp, ILI9225_DISP_CTRL1, 0x0012);
    delay_ms(50);
    disp_write_register(tft->disp, ILI9225_DISP_CTRL1, 0x1017);

    tft_setBacklight(tft, true);

    // Initialize variables
    tft_setBackgroundColor(tft, COLOR_BLACK);
    tft_setForegroundColor(tft, COLOR_WHITE);

    tft_clear(tft);
}

void tft_clear(struct tft_t *tft) {
    tft_fillRectangle(tft, 0, 0, tft->maxX - 1, tft->maxY - 1, COLOR_BLACK);
    delay_ms(10);
}

void tft_setBacklight(struct tft_t *tft, bool flag) {
    disp_set_backlight(tft->disp, flag);
}

void tft_setDisplay(struct tft_t *tft, bool flag) {
    if (flag) {
        disp_write_register(tft->disp, 0x00ff, 0x0000);
        disp_write_register(tft->disp, ILI9225_POWER_CTRL1, 0x0000);
        delay_ms(50);
        disp_write_register(tft->disp, ILI9225_DISP_CTRL1, 0x1017);
        delay_ms(200);
    } else {
        disp_write_register(tft->disp, 0x00ff, 0x0000);
        disp_write_register(tft->disp, ILI9225_DISP_CTRL1, 0x0000);
        delay_ms(50);
        disp_write_register(tft->disp, ILI9225_POWER_CTRL1, 0x0003);
        delay_ms(200);
    }
}

void tft_fillRectangle(struct tft_t *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    setWindow(tft, x1, y1, x2, y2);

    for(uint16_t t=(y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        disp_write_data(tft->disp, color);
    }
}

void tft_setBackgroundColor(struct tft_t *tft, uint16_t color) {
    tft->bgColor = color;
}

void tft_setForegroundColor(struct tft_t *tft, uint16_t color) {
    tft->fgColor = color;
}

uint16_t tft_drawChar(struct tft_t *tft, uint8_t x, uint8_t y, char ch) {
    uint16_t pixel_x = tft->maxX - (uint16_t)(y + 1) * tft->cfont->height;
    uint16_t pixel_y = (uint16_t)x * tft->cfont->width;
    setWindow(tft, pixel_x, pixel_y, pixel_x + tft->cfont->height - 1, pixel_y + tft->cfont->width - 1);
    uint16_t charPixels[6 * 8] = {0};
    for (uint8_t i = 0; i < tft->cfont->width; i++) {  // each font "column"
        uint8_t charData = font_read_column(tft->cfont, ch, i);

        // Process every row in font character
        for (uint8_t k = 0; k < tft->cfont->height; k++) {
            uint8_t j = tft->cfont->height - k - 1;
            if (charData & 1) {
                charPixels[i * 8 + j] = tft->fgColor;
            } else {
                charPixels[i * 8 + j] = tft->bgColor;
            }
            charData >>= 1;
        }
    }
    for (uint8_t i = 0; i < (6 * 8); i++) {
        disp_write_data(tft->disp, charPixels[i]);
    }
    return tft->cfont->width;
}
