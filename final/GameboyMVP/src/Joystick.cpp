#include "Joystick.h"

Joystick::Joystick(uint8_t xPin, uint8_t yPin, int adcMax, int deadzone)
    : xPin_(xPin),
      yPin_(yPin),
      adcMax_(adcMax),
      deadzone_(deadzone),
      centerX_(adcMax / 2),
  centerY_(adcMax / 2),
  minX_(adcMax),
  maxX_(0),
  minY_(adcMax),
  maxY_(0) {}

void Joystick::begin() {
  pinMode(xPin_, INPUT);
  pinMode(yPin_, INPUT);
}

void Joystick::calibrateCenter(uint8_t samples) {
  if (samples == 0) {
    return;
  }

  long xSum = 0;
  long ySum = 0;
  for (uint8_t i = 0; i < samples; ++i) {
    xSum += readAxisAveraged(xPin_);
    ySum += readAxisAveraged(yPin_);
    delay(2);
  }

  centerX_ = static_cast<int>(xSum / samples);
  centerY_ = static_cast<int>(ySum / samples);

  minX_ = centerX_;
  maxX_ = centerX_;
  minY_ = centerY_;
  maxY_ = centerY_;
}

Joystick::RawReading Joystick::readRaw() const {
  return {readAxisAveraged(xPin_), readAxisAveraged(yPin_)};
}

Joystick::NormalizedReading Joystick::readNormalized() {
  RawReading reading = readRaw();
  updateObservedRange(reading);

  return {
      normalizeAxis(reading.x, centerX_, minX_, maxX_),
      normalizeAxis(reading.y, centerY_, minY_, maxY_),
  };
}

float Joystick::normalizeAxis(int raw, int center, int minObserved,
                              int maxObserved) const {
  int delta = raw - center;
  if (abs(delta) <= deadzone_) {
    return 0.0f;
  }

  int positiveRange = max(maxObserved - center, 1);
  int negativeRange = max(center - minObserved, 1);

  float value = 0.0f;
  if (delta > 0) {
    value = static_cast<float>(delta) / static_cast<float>(positiveRange);
  } else {
    value = static_cast<float>(delta) / static_cast<float>(negativeRange);
  }

  return constrain(value, -1.0f, 1.0f);
}

int Joystick::readAxisAveraged(uint8_t pin, uint8_t samples) const {
  if (samples == 0) {
    return analogRead(pin);
  }

  uint32_t sum = 0;
  for (uint8_t i = 0; i < samples; ++i) {
    sum += analogRead(pin);
  }

  return static_cast<int>(sum / samples);
}

void Joystick::updateObservedRange(const RawReading& reading) {
  if (reading.x < minX_) {
    minX_ = reading.x;
  }
  if (reading.x > maxX_) {
    maxX_ = reading.x;
  }
  if (reading.y < minY_) {
    minY_ = reading.y;
  }
  if (reading.y > maxY_) {
    maxY_ = reading.y;
  }
}
