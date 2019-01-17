#include "delay.h"
#include "dispif.h"
#include "display.h"
#include "panic.h"

#define DISPLAY_COLS (176u)
#define DISPLAY_ROWS (220u)

#define REG_DRIVER_CODE_READ (0x00)

#define REG_DRIVER_OUTPUT_CONTROL (0x01)
#define BIT_VSPL (15)
#define BIT_HSPL (14)
#define BIT_DPL  (13)
#define BIT_EPL  (12)
#define BIT_SM   (10)
#define BIT_GS    (9)
#define BIT_SS    (8)
#define BIT_NL4   (4)
#define BIT_NL3   (3)
#define BIT_NL2   (2)
#define BIT_NL1   (1)
#define BIT_NL0   (0)

#define REG_LCD_AC_DRIVING_CONTROL (0x02)
#define BIT_INV1 (9)
#define BIT_INV0 (8)
#define BIT_FLD  (0)

#define REG_ENTRY_MODE (0x03)
#define BIT_BGR  (12)
#define BIT_MTD1  (9)
#define BIT_MTD0  (8)
#define BIT_ID1   (5)
#define BIT_ID0   (4)
#define BIT_AM    (3)

#define REG_DISPLAY_CONTROL_1 (0x07)
#define BIT_TEMON (12)
#define BIT_GON    (4)
#define BIT_CL     (3)
#define BIT_REV    (2)
#define BIT_D1     (1)
#define BIT_D0     (0)

#define REG_BLANK_PERIOD_CONTROL_1 (0x08)
#define BIT_FP3 (11)
#define BIT_FP2 (10)
#define BIT_FP1  (9)
#define BIT_FP0  (8)
#define BIT_BP3  (3)
#define BIT_BP2  (2)
#define BIT_BP1  (1)
#define BIT_BP0  (0)

#define REG_FRAME_CYCLE_CONTROL (0x0B)
#define BIT_NO3  (15)
#define BIT_NO2  (14)
#define BIT_NO1  (13)
#define BIT_NO0  (12)
#define BIT_STD3 (11)
#define BIT_STD2 (10)
#define BIT_STD1  (9)
#define BIT_STD0  (8)
#define BIT_RTN3  (3)
#define BIT_RTN2  (2)
#define BIT_RTN1  (1)
#define BIT_RTN0  (0)

#define REG_INTERFACE_CONTROL (0x0C)
#define BIT_RM   (8)
#define BIT_DM   (4)
#define BIT_RIM1 (1)
#define BIT_RIM0 (1)

#define REG_OSCILLATION_CONTROL (0x0F)
#define BIT_FOSC3 (11)
#define BIT_FOSC2 (10)
#define BIT_FOSC1  (9)
#define BIT_FOSC0  (8)
#define BIT_OSCON  (0)

#define REG_POWER_CONTROL_1 (0x10)
#define BIT_SAP3 (11)
#define BIT_SAP2 (10)
#define BIT_SAP1  (9)
#define BIT_SAP0  (8)
#define BIT_DSTB  (1)
#define BIT_STB   (0)

#define REG_POWER_CONTROL_2 (0x11)
#define BIT_APON   (12)
#define BIT_PON3   (11)
#define BIT_PON2   (10)
#define BIT_PON1    (9)
#define BIT_PON0    (8)
#define BIT_AON     (5)
#define BIT_VCI1EN  (4)
#define BIT_VC3     (3)
#define BIT_VC2     (2)
#define BIT_VC1     (1)
#define BIT_VC0     (0)

#define REG_POWER_CONTROL_3 (0x12)
#define BIT_BT2  (14)
#define BIT_BT1  (13)
#define BIT_BT0  (12)
#define BIT_DC12 (10)
#define BIT_DC11  (9)
#define BIT_DC10  (8)
#define BIT_DC22  (6)
#define BIT_DC21  (5)
#define BIT_DC20  (4)
#define BIT_DC32  (2)
#define BIT_DC31  (1)
#define BIT_DC30  (0)

#define REG_POWER_CONTROL_4 (0x13)
#define BIT_GVD6 (6)
// ...
#define BIT_GVD0 (0)

#define REG_POWER_CONTROL_5 (0x14)
#define BIT_VCOMG (15)
#define BIT_VCM6 (14)
// ...
#define BIT_VCM0 (8)
#define BIT_VML6 (6)
// ...
#define BIT_VML0 (0)

#define REG_CURRENT_COLUMN (0x20)
#define REG_CURRENT_ROW    (0x21)
#define REG_WRITE_TO_GRAM  (0x22)
#define REG_SOFTWARE_RESET (0x28)

#define REG_GATE_SCAN_CONTROL   (0x30)
#define REG_SCROLL_ROW_END      (0x31)
#define REG_SCROLL_ROW_START    (0x32)
#define REG_SCROLL_ROW_STEPS    (0x33)
#define REG_PARTIAL_ROW_END     (0x34)
#define REG_PARTIAL_ROW_START   (0x35)
#define REG_WINDOW_COLUMN_END   (0x36)
#define REG_WINDOW_COLUMN_START (0x37)
#define REG_WINDOW_ROW_END      (0x38)
#define REG_WINDOW_ROW_START    (0x39)

#define REG_GAMMA_CONTROL_1  (0x50)
#define REG_GAMMA_CONTROL_2  (0x51)
#define REG_GAMMA_CONTROL_3  (0x52)
#define REG_GAMMA_CONTROL_4  (0x53)
#define REG_GAMMA_CONTROL_5  (0x54)
#define REG_GAMMA_CONTROL_6  (0x55)
#define REG_GAMMA_CONTROL_7  (0x56)
#define REG_GAMMA_CONTROL_8  (0x57)
#define REG_GAMMA_CONTROL_9  (0x58)
#define REG_GAMMA_CONTROL_10 (0x59)

void display_init(struct display_t *display, struct dispif_t *dispif, enum orientation_t orientation, enum write_order_t write_order)
{
    display->dispif = dispif;
    display->orientation = orientation;

    // This procedure is based on the "Hydis 2.2 inch Panel Initial Code"
    // from the application note.
    dispif_set_backlight(dispif, false);
    delay_ms(1);
    dispif_set_reset(dispif, false);
    delay_us(10);
    dispif_set_reset(dispif, true);
    delay_ms(50);

    // Power on sequence
    dispif_write_register(dispif, REG_POWER_CONTROL_1,
        (0xA<<BIT_SAP0) |
        (0<<BIT_DSTB)   |
        (0<<BIT_STB)
    );
    dispif_write_register(dispif, REG_POWER_CONTROL_2,
        (1<<BIT_APON)   |
        (0x0<<BIT_PON0) |
        (1<<BIT_AON)    |
        (1<<BIT_VCI1EN) |
        (0xB<<BIT_VC0)
    );
    delay_ms(50);
    dispif_write_register(dispif, REG_POWER_CONTROL_3,
        (0x6<<BIT_BT0)  |
        (0x1<<BIT_DC10) |
        (0x2<<BIT_DC20) |
        (0x1<<BIT_DC30)
    );
    dispif_write_register(dispif, REG_POWER_CONTROL_4,
        (0x6F<<BIT_GVD0)
    );
    dispif_write_register(dispif, REG_POWER_CONTROL_5,
        (0<<BIT_VCOMG)   |
        (0x49<<BIT_VCM0) |
        (0x5F<<BIT_VML0)
    );

    // Rest of configuration
    dispif_write_register(dispif, REG_DRIVER_OUTPUT_CONTROL,
        (0<<BIT_VSPL) |
        (0<<BIT_HSPL) |
        (0<<BIT_DPL)  |
        (0<<BIT_EPL)  |
        (0<<BIT_SM)   |
        (0<<BIT_GS)   |
        (1<<BIT_SS)   |
        (0x1C<<BIT_NL0)
    );
    dispif_write_register(dispif, REG_LCD_AC_DRIVING_CONTROL,
        (0<<BIT_INV1) |
        (1<<BIT_INV0) |
        (0<<BIT_FLD)
    );
    uint8_t inc_row, inc_col, column_major;
    switch (orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
        inc_row = 1;
        inc_col = 1;
        column_major = (write_order == WRITE_ORDER_Y_MAJOR);
        break;
    case ORIENTATION_RIBBON_LEFT:
        inc_row = 0;
        inc_col = 1;
        column_major = (write_order == WRITE_ORDER_X_MAJOR);
        break;
    case ORIENTATION_RIBBON_TOP:
        inc_row = 0;
        inc_col = 0;
        column_major = (write_order == WRITE_ORDER_Y_MAJOR);
        break;
    case ORIENTATION_RIBBON_RIGHT:
        inc_row = 1;
        inc_col = 0;
        column_major = (write_order == WRITE_ORDER_X_MAJOR);
        break;
    default:
        panic();
        break;
    }
    dispif_write_register(dispif, REG_ENTRY_MODE,
        (1<<BIT_BGR)  |
        (0<<BIT_MTD1) |
        (0<<BIT_MTD0) |
        (inc_row<<BIT_ID1)  |
        (inc_col<<BIT_ID0)  |
        (column_major<<BIT_AM)
    );
    dispif_write_register(dispif, REG_DISPLAY_CONTROL_1,
        (0<<BIT_TEMON) |
        (1<<BIT_GON)   |
        (0<<BIT_CL)    |
        (1<<BIT_REV)   |
        (1<<BIT_D1)    |
        (1<<BIT_D0)
    );
    dispif_write_register(dispif, REG_BLANK_PERIOD_CONTROL_1,
        (0x8<<BIT_FP0) |
        (0x8<<BIT_BP0)
    );
    dispif_write_register(dispif, REG_FRAME_CYCLE_CONTROL,
        (0x1<<BIT_NO0)  |
        (0x1<<BIT_STD0) |
        (0x0<<BIT_RTN0)
    );
    dispif_write_register(dispif, REG_INTERFACE_CONTROL,
        (0<<BIT_RM)   |
        (0<<BIT_DM)   |
        (0<<BIT_RIM1) |
        (0<<BIT_RIM0)
    );
    dispif_write_register(dispif, REG_FRAME_CYCLE_CONTROL,
        (0x8<<BIT_FOSC0) |
        (1<<BIT_OSCON)
    );

    dispif_write_register(dispif, REG_CURRENT_COLUMN, 0x0000);
    dispif_write_register(dispif, REG_CURRENT_ROW,    0x0000);

    dispif_write_register(dispif, REG_GATE_SCAN_CONTROL,   0x0000);
    dispif_write_register(dispif, REG_SCROLL_ROW_END,      0x00DB);
    dispif_write_register(dispif, REG_SCROLL_ROW_START,    0x0000);
    dispif_write_register(dispif, REG_SCROLL_ROW_STEPS,    0x0000);
    dispif_write_register(dispif, REG_PARTIAL_ROW_END,     0x00DB);
    dispif_write_register(dispif, REG_PARTIAL_ROW_START,   0x0000);
    dispif_write_register(dispif, REG_PARTIAL_ROW_END,     0x00DB);
    dispif_write_register(dispif, REG_PARTIAL_ROW_START,   0x0000);
    dispif_write_register(dispif, REG_WINDOW_COLUMN_END,   0x00AF);
    dispif_write_register(dispif, REG_WINDOW_COLUMN_START, 0x0000);
    dispif_write_register(dispif, REG_WINDOW_ROW_END,      0x00DB);
    dispif_write_register(dispif, REG_WINDOW_ROW_START,    0x0000);

    dispif_write_register(dispif, REG_GAMMA_CONTROL_1,  0x0000);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_2,  0x0808);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_3,  0x080A);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_4,  0x080A);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_5,  0x000A);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_6,  0x0A08);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_7,  0x0808);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_8,  0x0000);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_9,  0x1007);
    dispif_write_register(dispif, REG_GAMMA_CONTROL_10, 0x0710);
}

void display_activate(struct display_t *display)
{
    dispif_set_backlight(display->dispif, true);
}

uint8_t display_get_width(struct display_t *display)
{
    switch (display->orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
    case ORIENTATION_RIBBON_TOP:
        return DISPLAY_COLS;
    case ORIENTATION_RIBBON_LEFT:
    case ORIENTATION_RIBBON_RIGHT:
        return DISPLAY_ROWS;
    default:
        panic();
        break;
    }
}

uint8_t display_get_height(struct display_t *display)
{
    switch (display->orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
    case ORIENTATION_RIBBON_TOP:
        return DISPLAY_ROWS;
    case ORIENTATION_RIBBON_LEFT:
    case ORIENTATION_RIBBON_RIGHT:
        return DISPLAY_COLS;
    default:
        panic();
        break;
    }
}

void display_set_window(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
    struct dispif_t *dispif = display->dispif;
    uint8_t start_col, start_row, end_col, end_row, current_col, current_row;

    switch (display->orientation) {
    case ORIENTATION_RIBBON_BOTTOM:
        start_col   = x;
        end_col     = x + w - 1;
        current_col = x;
        start_row   = y;
        end_row     = y + h - 1;
        current_row = y;
        break;
    case ORIENTATION_RIBBON_LEFT:
        start_col   = y;
        end_col     = y + h - 1;
        current_col = y;
        start_row   = DISPLAY_ROWS - x - w;
        end_row     = DISPLAY_ROWS - x - 1;
        current_row = DISPLAY_ROWS - x - 1;
        break;
    case ORIENTATION_RIBBON_TOP:
        start_col   = DISPLAY_COLS - x - w;
        end_col     = DISPLAY_COLS - x - 1;
        current_col = DISPLAY_COLS - x - 1;
        start_row   = DISPLAY_ROWS - y - h;
        end_row     = DISPLAY_ROWS - y - 1;
        current_row = DISPLAY_ROWS - y - 1;
        break;
    case ORIENTATION_RIBBON_RIGHT:
        start_col   = DISPLAY_COLS - y - h;
        end_col     = DISPLAY_COLS - y - 1;
        current_col = DISPLAY_COLS - y - 1;
        start_row   = x;
        end_row     = x + w - 1;
        current_row = x;
        break;
    default:
        panic();
        break;
    }

    dispif_write_register(dispif, REG_WINDOW_COLUMN_START, start_col);
    dispif_write_register(dispif, REG_WINDOW_COLUMN_END, end_col);
    dispif_write_register(dispif, REG_WINDOW_ROW_START, start_row);
    dispif_write_register(dispif, REG_WINDOW_ROW_END, end_row);
    dispif_write_register(dispif, REG_CURRENT_COLUMN, current_col);
    dispif_write_register(dispif, REG_CURRENT_ROW, current_row);
    dispif_write_command(dispif, REG_WRITE_TO_GRAM);
}

void display_write_pixel(struct display_t *display, uint16_t color)
{
    dispif_write_data(display->dispif, color);
}

void display_fill_rectangle(struct display_t *display, uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
    display_set_window(display, x, y, w, h);
    for (uint16_t i = 0; i < (uint16_t) w * (uint16_t) h; i++) {
        dispif_write_data(display->dispif, color);
    }
}

void display_clear(struct display_t *display, uint16_t color)
{
    uint8_t w = display_get_width(display);
    uint8_t h = display_get_height(display);
    display_fill_rectangle(display, 0, 0, w, h, color);
}
