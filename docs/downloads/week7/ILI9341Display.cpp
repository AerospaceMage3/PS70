#include "ILI9341Display.h"

ILI9341Display::ILI9341Display(uint8_t sckPin, uint8_t mosiPin,
                               uint8_t backlightPin, uint8_t dcPin,
                               uint8_t csPin, uint8_t rstPin)
    : sckPin_(sckPin),
      mosiPin_(mosiPin),
      backlightPin_(backlightPin),
      dcPin_(dcPin),
      csPin_(csPin),
      rstPin_(rstPin) {
  tft_ = nullptr;
}

void ILI9341Display::begin(uint8_t rotation) {
  if (tft_ == nullptr) {
    tft_ = new Adafruit_ILI9341(csPin_, dcPin_, mosiPin_, sckPin_, rstPin_, -1);
  }

  pinMode(backlightPin_, OUTPUT);
  digitalWrite(backlightPin_, HIGH);

  tft_->begin(10000000);
  tft_->setRotation(rotation);
  tft_->fillScreen(ILI9341_BLACK);
}

void ILI9341Display::setBacklight(bool enabled) {
  digitalWrite(backlightPin_, enabled ? HIGH : LOW);
}

void ILI9341Display::clear(uint16_t color) {
  if (tft_ == nullptr) {
    return;
  }
  tft_->fillScreen(color);
}

void ILI9341Display::fillScreen(uint16_t color) {
  if (tft_ == nullptr) {
    return;
  }
  tft_->fillScreen(color);
}

void ILI9341Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                              uint16_t color) {
  if (tft_ == nullptr) {
    return;
  }
  tft_->fillRect(x, y, w, h, color);
}

void ILI9341Display::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                              uint16_t color) {
  if (tft_ == nullptr) {
    return;
  }
  tft_->drawRect(x, y, w, h, color);
}

void ILI9341Display::drawText(int16_t x, int16_t y, const String& text,
                              uint16_t color, uint8_t textSize,
                              uint16_t bgColor) {
  if (tft_ == nullptr) {
    return;
  }
  tft_->setCursor(x, y);
  tft_->setTextColor(color, bgColor);
  tft_->setTextSize(textSize);
  tft_->print(text);
}

int16_t ILI9341Display::width() const {
  if (tft_ == nullptr) {
    return 0;
  }
  return tft_->width();
}

int16_t ILI9341Display::height() const {
  if (tft_ == nullptr) {
    return 0;
  }
  return tft_->height();
}
