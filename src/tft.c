#include <stdbool.h>

#include "delay.h"
#include "dispif.h"
#include "font.h"
#include "tft.h"

struct screen_t;

static void set_window(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

static void set_window(struct tft_t *tft, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    dispif_write_register(tft->dispif, 0x36u, x1);
    dispif_write_register(tft->dispif, 0x37u, x0);
    dispif_write_register(tft->dispif, 0x38u, y1);
    dispif_write_register(tft->dispif, 0x39u, y0);
    dispif_write_register(tft->dispif, 0x20u, x0);
    dispif_write_register(tft->dispif, 0x21u, y0);
    dispif_write_command(tft->dispif, 0x22u);
}

void tft_init(struct tft_t *tft, struct dispif_t *dispif, struct screen_t *scr, struct font_t *font) {
    tft->font = font;
    tft->dispif = dispif;
    tft->scr = scr;
    tft->max_x = TFT_WIDTH;
    tft->max_y = TFT_HEIGHT;
    tft->bg_color = COLOR_BLACK;
    tft->fg_color = COLOR_WHITE;
}

void tft_begin(struct tft_t *tft) {
    dispif_set_reset(tft->dispif, true);
    delay_ms(1);
    dispif_set_reset(tft->dispif, false);
    delay_ms(10);
    dispif_set_reset(tft->dispif, true);
    delay_ms(50);

    dispif_write_register(tft->dispif, 0x10u, 0x0000u);
    dispif_write_register(tft->dispif, 0x11u, 0x0000u);
    dispif_write_register(tft->dispif, 0x12u, 0x0000u);
    dispif_write_register(tft->dispif, 0x13u, 0x0000u);
    dispif_write_register(tft->dispif, 0x14u, 0x0000u);

    dispif_write_register(tft->dispif, 0x11u, 0x0018u);
    dispif_write_register(tft->dispif, 0x12u, 0x6121u);
    dispif_write_register(tft->dispif, 0x13u, 0x006Fu);
    dispif_write_register(tft->dispif, 0x14u, 0x495Fu);
    dispif_write_register(tft->dispif, 0x10u, 0x0800u);
    delay_ms(10);
    dispif_write_register(tft->dispif, 0x11u, 0x103Bu);
    delay_ms(50);

    dispif_write_register(tft->dispif, 0x01u, 0x011Cu);
    dispif_write_register(tft->dispif, 0x02u, 0x0100u);
    dispif_write_register(tft->dispif, 0x03u, 0x1030u);
    dispif_write_register(tft->dispif, 0x07u, 0x0000u);
    dispif_write_register(tft->dispif, 0x08u, 0x0808u);
    dispif_write_register(tft->dispif, 0x0Bu, 0x1100u);
    dispif_write_register(tft->dispif, 0x0Cu, 0x0000u);
    dispif_write_register(tft->dispif, 0x0Fu, 0x0D01u);
    dispif_write_register(tft->dispif, 0x15u, 0x0020u);
    dispif_write_register(tft->dispif, 0x20u, 0x0000u);
    dispif_write_register(tft->dispif, 0x21u, 0x0000u);

    dispif_write_register(tft->dispif, 0x30u, 0x0000u);
    dispif_write_register(tft->dispif, 0x31u, 0x00DBu);
    dispif_write_register(tft->dispif, 0x32u, 0x0000u);
    dispif_write_register(tft->dispif, 0x33u, 0x0000u);
    dispif_write_register(tft->dispif, 0x34u, 0x00DBu);
    dispif_write_register(tft->dispif, 0x35u, 0x0000u);
    dispif_write_register(tft->dispif, 0x36u, 0x00AFu);
    dispif_write_register(tft->dispif, 0x37u, 0x0000u);
    dispif_write_register(tft->dispif, 0x38u, 0x00DBu);
    dispif_write_register(tft->dispif, 0x39u, 0x0000u);

    dispif_write_register(tft->dispif, 0x50u, 0x0000u);
    dispif_write_register(tft->dispif, 0x51u, 0x0808u);
    dispif_write_register(tft->dispif, 0x52u, 0x080Au);
    dispif_write_register(tft->dispif, 0x53u, 0x000Au);
    dispif_write_register(tft->dispif, 0x54u, 0x0A08u);
    dispif_write_register(tft->dispif, 0x55u, 0x0808u);
    dispif_write_register(tft->dispif, 0x56u, 0x0000u);
    dispif_write_register(tft->dispif, 0x57u, 0x0A00u);
    dispif_write_register(tft->dispif, 0x58u, 0x0710u);
    dispif_write_register(tft->dispif, 0x59u, 0x0710u);

    dispif_write_register(tft->dispif, 0x07u, 0x0012u);
    delay_ms(50);
    dispif_write_register(tft->dispif, 0x07u, 0x1017u);
}

void tft_clear(struct tft_t *tft) {
    tft_fill_rectangle(tft, 0, 0, tft->max_x - 1, tft->max_y - 1, tft->bg_color);
}

void tft_set_backlight(struct tft_t *tft, bool flag) {
    dispif_set_backlight(tft->dispif, flag);
}

void tft_fill_rectangle(struct tft_t *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    set_window(tft, x1, y1, x2, y2);

    for(uint16_t t=(y2 - y1 + 1) * (x2 - x1 + 1); t > 0; t--) {
        dispif_write_data(tft->dispif, color);
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
        dispif_write_data(tft->dispif, charPixels[i]);
    }
}
