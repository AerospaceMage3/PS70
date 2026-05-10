#pragma once

#include <Arduino.h>

class Joystick {
public:
  Joystick(uint8_t xPin, uint8_t yPin) : _xPin(xPin), _yPin(yPin) {}

  void begin() const {
    pinMode(_xPin, INPUT);
    pinMode(_yPin, INPUT);
  }

  int readX() const {
    return analogRead(_xPin);
  }

  int readY() const {
    return analogRead(_yPin);
  }

  void calibrateCenter(uint16_t samples = 100, uint16_t sampleDelayMs = 5) {
    long sumX = 0;
    long sumY = 0;

    for (uint16_t i = 0; i < samples; ++i) {
      sumX += analogRead(_xPin);
      sumY += analogRead(_yPin);
      delay(sampleDelayMs);
    }

    _centerX = sumX / samples;
    _centerY = sumY / samples;
  }

  int centerX() const {
    return _centerX;
  }

  int centerY() const {
    return _centerY;
  }

  int centeredX(int rawValue) const {
    return rawValue - _centerX;
  }

  int centeredY(int rawValue) const {
    return rawValue - _centerY;
  }

  int axisStateX(int rawValue, int threshold = 250) const {
    return axisState(rawValue, _centerX, threshold);
  }

  int axisStateY(int rawValue, int threshold = 250) const {
    return axisState(rawValue, _centerY, threshold);
  }

  int axisState(int rawValue, int centerValue, int threshold = 250) const {
    int offset = rawValue - centerValue;

    if (offset > threshold) {
      return 1;
    }

    if (offset < -threshold) {
      return -1;
    }

    return 0;
  }

private:
  uint8_t _xPin;
  uint8_t _yPin;
  int _centerX = 4095 / 2;
  int _centerY = 4095 / 2;
};
