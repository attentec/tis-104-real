#include <stdbool.h>

#include "delay.h"
#include "disp.h"
#include "font.h"
#include "tft.h"

struct screen;

static void set_window(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

static void set_window(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    disp_write_register(tft->disp, 0x36u, x1);
    disp_write_register(tft->disp, 0x37u, x0);
    disp_write_register(tft->disp, 0x38u, y1);
    disp_write_register(tft->disp, 0x39u, y0);
    disp_write_register(tft->disp, 0x20u, x0);
    disp_write_register(tft->disp, 0x21u, y0);
    disp_write_command(tft->disp, 0x22u);
}

void tft_init(struct tft_t *tft, struct disp_t *disp, struct screen *scr, struct font *font) {
    tft->font = font;
    tft->disp = disp;
    tft->scr = scr;
    tft->max_x = TFT_WIDTH;
    tft->max_y = TFT_HEIGHT;
    tft->bg_color = COLOR_BLACK;
    tft->fg_color = COLOR_WHITE;
}

void tft_begin(struct tft_t *tft) {
    disp_set_reset(tft->disp, true);
    delay_ms(1);
    disp_set_reset(tft->disp, false);
    delay_ms(10);
    disp_set_reset(tft->disp, true);
    delay_ms(50);

    disp_write_register(tft->disp, 0x10u, 0x0000u);
    disp_write_register(tft->disp, 0x11u, 0x0000u);
    disp_write_register(tft->disp, 0x12u, 0x0000u);
    disp_write_register(tft->disp, 0x13u, 0x0000u);
    disp_write_register(tft->disp, 0x14u, 0x0000u);

    disp_write_register(tft->disp, 0x11u, 0x0018u);
    disp_write_register(tft->disp, 0x12u, 0x6121u);
    disp_write_register(tft->disp, 0x13u, 0x006Fu);
    disp_write_register(tft->disp, 0x14u, 0x495Fu);
    disp_write_register(tft->disp, 0x10u, 0x0800u);
    delay_ms(10);
    disp_write_register(tft->disp, 0x11u, 0x103Bu);
    delay_ms(50);

    disp_write_register(tft->disp, 0x01u, 0x011Cu);
    disp_write_register(tft->disp, 0x02u, 0x0100u);
    disp_write_register(tft->disp, 0x03u, 0x1030u);
    disp_write_register(tft->disp, 0x07u, 0x0000u);
    disp_write_register(tft->disp, 0x08u, 0x0808u);
    disp_write_register(tft->disp, 0x0Bu, 0x1100u);
    disp_write_register(tft->disp, 0x0Cu, 0x0000u);
    disp_write_register(tft->disp, 0x0Fu, 0x0D01u);
    disp_write_register(tft->disp, 0x15u, 0x0020u);
    disp_write_register(tft->disp, 0x20u, 0x0000u);
    disp_write_register(tft->disp, 0x21u, 0x0000u);

    disp_write_register(tft->disp, 0x30u, 0x0000u);
    disp_write_register(tft->disp, 0x31u, 0x00DBu);
    disp_write_register(tft->disp, 0x32u, 0x0000u);
    disp_write_register(tft->disp, 0x33u, 0x0000u);
    disp_write_register(tft->disp, 0x34u, 0x00DBu);
    disp_write_register(tft->disp, 0x35u, 0x0000u);
    disp_write_register(tft->disp, 0x36u, 0x00AFu);
    disp_write_register(tft->disp, 0x37u, 0x0000u);
    disp_write_register(tft->disp, 0x38u, 0x00DBu);
    disp_write_register(tft->disp, 0x39u, 0x0000u);

    disp_write_register(tft->disp, 0x50u, 0x0000u);
    disp_write_register(tft->disp, 0x51u, 0x0808u);
    disp_write_register(tft->disp, 0x52u, 0x080Au);
    disp_write_register(tft->disp, 0x53u, 0x000Au);
    disp_write_register(tft->disp, 0x54u, 0x0A08u);
    disp_write_register(tft->disp, 0x55u, 0x0808u);
    disp_write_register(tft->disp, 0x56u, 0x0000u);
    disp_write_register(tft->disp, 0x57u, 0x0A00u);
    disp_write_register(tft->disp, 0x58u, 0x0710u);
    disp_write_register(tft->disp, 0x59u, 0x0710u);

    disp_write_register(tft->disp, 0x07u, 0x0012u);
    delay_ms(50);
    disp_write_register(tft->disp, 0x07u, 0x1017u);

    tft_set_backlight(tft, true);

    tft_set_background_color(tft, COLOR_BLACK);
    tft_set_foreground_color(tft, COLOR_WHITE);

    tft_clear(tft);
}

void tft_clear(struct tft_t *tft) {
    tft_fill_rectangle(tft, 0, 0, tft->max_x - 1, tft->max_y - 1, COLOR_BLACK);
    delay_ms(10);
}

void tft_set_backlight(struct tft_t *tft, bool flag) {
    disp_set_backlight(tft->disp, flag);
}

void tft_fill_rectangle(struct tft_t *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    set_window(tft, x1, y1, x2, y2);

    for(uint16_t t=(y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        disp_write_data(tft->disp, color);
    }
}

void tft_set_background_color(struct tft_t *tft, uint16_t color) {
    tft->bg_color = color;
}

void tft_set_foreground_color(struct tft_t *tft, uint16_t color) {
    tft->fg_color = color;
}

void tft_draw_char(struct tft_t *tft, uint8_t x, uint8_t y, char ch) {
    uint16_t pixel_x = tft->max_x - (uint16_t)(y + 1) * tft->font->height;
    uint16_t pixel_y = (uint16_t)x * tft->font->width;
    set_window(tft, pixel_x, pixel_y, pixel_x + tft->font->height - 1, pixel_y + tft->font->width - 1);
    uint16_t charPixels[6 * 8] = {0};
    // For each column of font character
    for (uint8_t i = 0; i < tft->font->width; i++) {
        uint8_t charData = font_read_column(tft->font, ch, i);

        // For every row in font character
        for (uint8_t k = 0; k < tft->font->height; k++) {
            uint8_t j = tft->font->height - k - 1;
            if (charData & 1) {
                charPixels[i * 8 + j] = tft->fg_color;
            } else {
                charPixels[i * 8 + j] = tft->bg_color;
            }
            charData >>= 1;
        }
    }
    for (uint8_t i = 0; i < (6 * 8); i++) {
        disp_write_data(tft->disp, charPixels[i]);
    }
}
