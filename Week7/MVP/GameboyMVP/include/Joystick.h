#pragma once

#include <Arduino.h>

class Joystick {
 public:
  struct RawReading {
    int x;
    int y;
  };

  struct NormalizedReading {
    float x;
    float y;
  };

  Joystick(uint8_t xPin, uint8_t yPin, int adcMax = 4095, int deadzone = 0);

  void begin();
  void calibrateCenter(uint8_t samples = 16);

  RawReading readRaw() const;
  NormalizedReading readNormalized();

 private:
  float normalizeAxis(int raw, int center, int minObserved, int maxObserved) const;
  int readAxisAveraged(uint8_t pin, uint8_t samples = 2) const;
  void updateObservedRange(const RawReading& reading);

  uint8_t xPin_;
  uint8_t yPin_;
  int adcMax_;
  int deadzone_;
  int centerX_;
  int centerY_;
  int minX_;
  int maxX_;
  int minY_;
  int maxY_;
};
