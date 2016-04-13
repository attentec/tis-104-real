// Include application, user and local libraries
#include <stdlib.h>
#include <util/delay.h>

#include "TFT_22_ILI9225.h"

#define TFT_RST 2
#define TFT_RS  3
#define TFT_CS  4  // SS
#define TFT_SDI 5  // MOSI
#define TFT_CLK 6  // SCK
#define TFT_LED 7   // 0 if wired to +5V directly

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

uint16_t random(uint16_t max) {
  return (uint16_t)(((float)rand() * (float)max) / (float)RAND_MAX);
}

int main() {
  tft.begin();

  tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
  tft.setFont(Terminal6x8);
  tft.drawText(10, 10, "hello!");
  _delay_ms(1000);

  tft.clear();
  tft.drawText(10, 20, "clear");
  _delay_ms(1000);

  tft.drawText(10, 30, "text small");
  tft.setBackgroundColor(COLOR_YELLOW);
  tft.setFont(Terminal12x16);
  tft.drawText(90, 30, "BIG", COLOR_RED);
  tft.setBackgroundColor(COLOR_BLACK);
  tft.setFont(Terminal6x8);
  _delay_ms(1000);

  tft.drawText(10, 40, "setBacklight off");
  _delay_ms(500);
  tft.setBacklight(LOW);
  _delay_ms(500);
  tft.setBacklight(HIGH);
  tft.drawText(10, 50, "setBacklight on");
  _delay_ms(1000);

  tft.drawRectangle(10, 10, 110, 110, COLOR_BLUE);
  tft.drawText(10, 60, "rectangle");
  _delay_ms(1000);

  tft.fillRectangle(20, 20, 120, 120, COLOR_RED);
  tft.drawText(10, 70, "solidRectangle");
  _delay_ms(1000);

  tft.drawCircle(80, 80, 50, COLOR_YELLOW);
  tft.drawText(10, 80, "circle");
  _delay_ms(1000);

  tft.fillCircle(90, 90, 30, COLOR_GREEN);
  tft.drawText(10, 90, "solidCircle");
  _delay_ms(1000);

  tft.drawLine(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_CYAN);
  tft.drawText(10, 100, "line");
  _delay_ms(1000);

  for (uint8_t i = 0; i < 127; i++)
    tft.drawPixel(random(tft.maxX()), random(tft.maxY()), random(0xffff));
  tft.drawText(10, 110, "point");
  _delay_ms(1000);

  char *message = "setOrientation (0)";
  for (uint8_t i = 0; i < 4; i++) {
    tft.clear();
    tft.setOrientation(i);
    tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
    message[16] = '0' + i;
    tft.drawText(10, 10, message);
    tft.drawRectangle(10, 20, 50, 60, COLOR_GREEN);
    tft.drawCircle(70, 80, 10, COLOR_BLUE);
    tft.drawLine(30, 40, 70, 80, COLOR_YELLOW);
    _delay_ms(1000);
  }

  tft.setOrientation(0);
  tft.clear();
  tft.setFont(Terminal12x16);
  tft.setBackgroundColor(COLOR_YELLOW);
  tft.drawText(10, 40, "bye!", COLOR_RED);
  tft.setBackgroundColor(COLOR_BLACK);
  tft.setFont(Terminal6x8);
  _delay_ms(1000);

  tft.drawText(10, 60, "off");
  _delay_ms(1000);

  tft.setBacklight(false);
  tft.setDisplay(false);

  while(true);

  return 0;
}
