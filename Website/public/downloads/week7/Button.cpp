#include "Button.h"

Button::Button(uint8_t pin) : pin_(pin) {}

void Button::begin() {
  pinMode(pin_, INPUT_PULLUP);
}

bool Button::isPressed() const {
  return digitalRead(pin_) == LOW;
}
