#pragma once

#include <Arduino.h>

class Button {
 public:
  explicit Button(uint8_t pin);

  void begin();
  bool isPressed() const;

 private:
  uint8_t pin_;
};
