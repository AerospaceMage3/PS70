#pragma once

#include <Arduino.h>

class DriveMotor {
 public:
  DriveMotor(uint8_t forwardPin, uint8_t backwardPin)
      : forwardPin_(forwardPin), backwardPin_(backwardPin) {}

  void begin() {
    pinMode(forwardPin_, OUTPUT);
    pinMode(backwardPin_, OUTPUT);
    stop();
  }

  void forward() {
    digitalWrite(forwardPin_, HIGH);
    digitalWrite(backwardPin_, LOW);
  }

  void backward() {
    digitalWrite(forwardPin_, LOW);
    digitalWrite(backwardPin_, HIGH);
  }

  void stop() {
    digitalWrite(forwardPin_, LOW);
    digitalWrite(backwardPin_, LOW);
  }

  void updateFromDiscreteInput(int input) {
    if (input > 0) {
      forward();
      return;
    }

    if (input < 0) {
      backward();
      return;
    }

    stop();
  }

 private:
  uint8_t forwardPin_;
  uint8_t backwardPin_;
};