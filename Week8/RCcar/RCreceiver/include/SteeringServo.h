#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

class SteeringServo {
 public:
  SteeringServo(uint8_t pin,
                int minAngle = 0,
                int centerAngle = 90,
                int maxAngle = 180,
                int minPulseUs = 500,
                int maxPulseUs = 2400)
      : pin_(pin),
        minAngle_(minAngle),
        centerAngle_(centerAngle),
        maxAngle_(maxAngle),
        minPulseUs_(minPulseUs),
        maxPulseUs_(maxPulseUs),
        currentAngle_(centerAngle) {}

  void begin() {
    servo_.setPeriodHertz(50);
    servo_.attach(pin_, minPulseUs_, maxPulseUs_);
    writeAngle(centerAngle_);
  }

  void updateFromInput(int input, int inputMin, int inputMax) {
    if (inputMax <= inputMin) {
      writeAngle(centerAngle_);
      return;
    }

    int clampedInput = constrain(input, inputMin, inputMax);
    int mappedAngle = map(clampedInput, inputMin, inputMax, minAngle_, maxAngle_);
    writeAngle(mappedAngle);
  }

  void updateFromDiscreteInput(int input) {
    if (input > 0) {
      writeAngle(maxAngle_);
      return;
    }

    if (input < 0) {
      writeAngle(minAngle_);
      return;
    }

    writeAngle(centerAngle_);
  }

  void writeAngle(int angle) {
    currentAngle_ = constrain(angle, minAngle_, maxAngle_);
    servo_.write(currentAngle_);
  }

  int currentAngle() const { return currentAngle_; }

 private:
  Servo servo_;
  uint8_t pin_;
  int minAngle_;
  int centerAngle_;
  int maxAngle_;
  int minPulseUs_;
  int maxPulseUs_;
  int currentAngle_;
};