#pragma once

#include <Arduino.h>

class MicController {
 public:
  MicController(int micPin, int motorPin);

  void begin();
  void update();

 private:
  int micPin_;
  int motorPin_;
  unsigned long windowStartMs_ = 0;
  unsigned int signalMax_ = 0;
  unsigned int signalMin_ = 1024;
  bool samplingWindowActive_ = false;
  static constexpr int sampleWindowMs_ = 50;
  static constexpr unsigned int threshold_ = 700;
};