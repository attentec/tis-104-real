#pragma once

#include <stdbool.h>

#include "disp.h"
#include "font.h"
#include "pin.h"
#include "screen.h"

/* ILI9225 screen size */
#define ILI9225_LCD_WIDTH  176
#define ILI9225_LCD_HEIGHT 220

/* ILI9225 LCD Registers */
#define ILI9225_DRIVER_OUTPUT_CTRL      (0x01u)  // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL     (0x02u)  // LCD AC Driving Control
#define ILI9225_ENTRY_MODE              (0x03u)  // Entry Mode
#define ILI9225_DISP_CTRL1              (0x07u)  // Display Control 1
#define ILI9225_BLANK_PERIOD_CTRL1      (0x08u)  // Blank Period Control
#define ILI9225_FRAME_CYCLE_CTRL        (0x0Bu)  // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL          (0x0Cu)  // Interface Control
#define ILI9225_OSC_CTRL                (0x0Fu)  // Osc Control
#define ILI9225_POWER_CTRL1             (0x10u)  // Power Control 1
#define ILI9225_POWER_CTRL2             (0x11u)  // Power Control 2
#define ILI9225_POWER_CTRL3             (0x12u)  // Power Control 3
#define ILI9225_POWER_CTRL4             (0x13u)  // Power Control 4
#define ILI9225_POWER_CTRL5             (0x14u)  // Power Control 5
#define ILI9225_VCI_RECYCLING           (0x15u)  // VCI Recycling
#define ILI9225_RAM_ADDR_SET1           (0x20u)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           (0x21u)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG           (0x22u)  // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL          (0x30u)  // Gate Scan Control Register
#define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31u)  // Vertical Scroll Control 1 Register
#define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32u)  // Vertical Scroll Control 2 Register
#define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33u)  // Vertical Scroll Control 3 Register
#define ILI9225_PARTIAL_DRIVING_POS1    (0x34u)  // Partial Driving Position 1 Register
#define ILI9225_PARTIAL_DRIVING_POS2    (0x35u)  // Partial Driving Position 2 Register
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36u)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 (0x37u)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38u)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39u)  // Vertical Address End Position
#define ILI9225_GAMMA_CTRL1             (0x50u)  // Gamma Control 1
#define ILI9225_GAMMA_CTRL2             (0x51u)  // Gamma Control 2
#define ILI9225_GAMMA_CTRL3             (0x52u)  // Gamma Control 3
#define ILI9225_GAMMA_CTRL4             (0x53u)  // Gamma Control 4
#define ILI9225_GAMMA_CTRL5             (0x54u)  // Gamma Control 5
#define ILI9225_GAMMA_CTRL6             (0x55u)  // Gamma Control 6
#define ILI9225_GAMMA_CTRL7             (0x56u)  // Gamma Control 7
#define ILI9225_GAMMA_CTRL8             (0x57u)  // Gamma Control 8
#define ILI9225_GAMMA_CTRL9             (0x58u)  // Gamma Control 9
#define ILI9225_GAMMA_CTRL10            (0x59u)  // Gamma Control 10

#define ILI9225C_INVOFF  0x20
#define ILI9225C_INVON   0x21


/* RGB 24-bits color table definition (RGB888). */
#define RGB888_RGB565(color) ((((color) >> 19) & 0x1f) << 11) | ((((color) >> 10) & 0x3f) << 5) | (((color) >> 3) & 0x1f)

#define COLOR_BLACK          RGB888_RGB565(0x000000uL)
#define COLOR_WHITE          RGB888_RGB565(0xFFFFFFuL)
#define COLOR_BLUE           RGB888_RGB565(0x0000FFuL)
#define COLOR_GREEN          RGB888_RGB565(0x00FF00uL)
#define COLOR_RED            RGB888_RGB565(0xFF0000uL)
#define COLOR_NAVY           RGB888_RGB565(0x000080uL)
#define COLOR_DARKBLUE       RGB888_RGB565(0x00008BuL)
#define COLOR_DARKGREEN      RGB888_RGB565(0x006400uL)
#define COLOR_DARKCYAN       RGB888_RGB565(0x008B8BuL)
#define COLOR_CYAN           RGB888_RGB565(0x00FFFFuL)
#define COLOR_TURQUOISE      RGB888_RGB565(0x40E0D0uL)
#define COLOR_INDIGO         RGB888_RGB565(0x4B0082uL)
#define COLOR_DARKRED        RGB888_RGB565(0x800000uL)
#define COLOR_OLIVE          RGB888_RGB565(0x808000uL)
#define COLOR_GRAY           RGB888_RGB565(0x808080uL)
#define COLOR_SKYBLUE        RGB888_RGB565(0x87CEEBuL)
#define COLOR_BLUEVIOLET     RGB888_RGB565(0x8A2BE2uL)
#define COLOR_LIGHTGREEN     RGB888_RGB565(0x90EE90uL)
#define COLOR_DARKVIOLET     RGB888_RGB565(0x9400D3uL)
#define COLOR_YELLOWGREEN    RGB888_RGB565(0x9ACD32uL)
#define COLOR_BROWN          RGB888_RGB565(0xA52A2AuL)
#define COLOR_DARKGRAY       RGB888_RGB565(0xA9A9A9uL)
#define COLOR_SIENNA         RGB888_RGB565(0xA0522DuL)
#define COLOR_LIGHTBLUE      RGB888_RGB565(0xADD8E6uL)
#define COLOR_GREENYELLOW    RGB888_RGB565(0xADFF2FuL)
#define COLOR_SILVER         RGB888_RGB565(0xC0C0C0uL)
#define COLOR_LIGHTGREY      RGB888_RGB565(0xD3D3D3uL)
#define COLOR_LIGHTCYAN      RGB888_RGB565(0xE0FFFFuL)
#define COLOR_VIOLET         RGB888_RGB565(0xEE82EEuL)
#define COLOR_AZUR           RGB888_RGB565(0xF0FFFFuL)
#define COLOR_BEIGE          RGB888_RGB565(0xF5F5DCuL)
#define COLOR_MAGENTA        RGB888_RGB565(0xFF00FFuL)
#define COLOR_TOMATO         RGB888_RGB565(0xFF6347uL)
#define COLOR_GOLD           RGB888_RGB565(0xFFD700uL)
#define COLOR_ORANGE         RGB888_RGB565(0xFFA500uL)
#define COLOR_SNOW           RGB888_RGB565(0xFFFAFAuL)
#define COLOR_YELLOW         RGB888_RGB565(0xFFFF00uL)

struct tft_t {
    uint16_t max_x;
    uint16_t max_y;
    uint16_t bg_color;
    uint16_t fg_color;

    struct font *cfont;
    struct disp_t *disp;
    struct screen *scr;
};

void tft_init(struct tft_t *tft, struct disp_t *disp, struct screen *scr, struct font *font);
void tft_begin(struct tft_t *tft);
void tft_clear(struct tft_t *tft);
void tft_set_backlight(struct tft_t *tft, bool flag);
void tft_set_display(struct tft_t *tft, bool flag);
void tft_set_background_color(struct tft_t *tft, uint16_t color);
void tft_set_foreground_color(struct tft_t *tft, uint16_t color);
void tft_fill_rectangle(struct tft_t *tft, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
uint16_t tft_draw_char(struct tft_t *tft, uint8_t x, uint8_t y, char ch);
