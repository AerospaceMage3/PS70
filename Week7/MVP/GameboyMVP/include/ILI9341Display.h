#pragma once

#include <Adafruit_ILI9341.h>
#include <Arduino.h>
#include <SPI.h>

class ILI9341Display {
 public:
  ILI9341Display(uint8_t sckPin = D8, uint8_t mosiPin = D10,
                 uint8_t backlightPin = D4, uint8_t dcPin = D5,
                 uint8_t csPin = D6, uint8_t rstPin = D3);

  void begin(uint8_t rotation = 1);
  void setBacklight(bool enabled);
  void clear(uint16_t color = ILI9341_BLACK);
  void fillScreen(uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void drawText(int16_t x, int16_t y, const String& text,
                uint16_t color = ILI9341_WHITE, uint8_t textSize = 2,
                uint16_t bgColor = ILI9341_BLACK);

  int16_t width() const;
  int16_t height() const;

 private:
  uint8_t sckPin_;
  uint8_t mosiPin_;
  uint8_t backlightPin_;
  uint8_t dcPin_;
  uint8_t csPin_;
  uint8_t rstPin_;
  Adafruit_ILI9341* tft_;
};
