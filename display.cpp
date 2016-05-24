// Include application, user and local libraries
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include <TFT_22_ILI9225.h>

extern "C" {
#include <font.h>
#include <indexmap.h>
#include <screen.h>
}

#define TFT_RST 2
#define TFT_RS  3
#define TFT_CS  4  // SS
#define TFT_SDI 5  // MOSI
#define TFT_CLK 6  // SCK
#define TFT_LED 7   // 0 if wired to +5V directly

uint16_t random(uint16_t max) {
  return (uint16_t)(((float)rand() * (float)max) / (float)RAND_MAX);
}

int main() {
  indexmap indices = indexmap_init(20, 20);
  FontInfo font = font_make_fontinfo(Terminal6x8);
  screen scr = screen_init(indices, &font);

  // Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
  //TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
  // Use software SPI (slower)
  TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED, scr);

  tft.setFont(&font);

  tft.begin();

  screen_set(scr, 0, 0, 0x63);
  screen_set(scr, 19, 0, 0x62);
  screen_set(scr, 0, 19, 0x65);
  screen_set(scr, 19, 19, 0x64);
  for (int i = 1; i < 19; ++i) {
      screen_set(scr, i, 0, 0x60);
      screen_set(scr, i, 19, 0x60);
      screen_set(scr, 0, i, 0x61);
      screen_set(scr, 19, i, 0x61);
  }
  tft.render();

  int count = 0;
  char buffer[5];
  while (true) {
      sprintf(buffer, "%04d", count);
      buffer[4] = '\0';
      tft.drawText(10, 10, buffer);
      tft.render();
      count += 1;
  }

  tft.setBacklight(false);
  tft.setDisplay(false);


  return 0;
}
