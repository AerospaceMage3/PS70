#include "mic.h"

MicController::MicController(int micPin, int motorPin)
      : micPin_(micPin), motorPin_(motorPin) {}

void MicController::begin() {
   pinMode(motorPin_, OUTPUT);
   digitalWrite(motorPin_, LOW);
   samplingWindowActive_ = false;
}

void MicController::update() {
   unsigned long now = millis();

   if (!samplingWindowActive_) {
      windowStartMs_ = now;
      signalMax_ = 0;
      signalMin_ = 1024;
      samplingWindowActive_ = true;
   }

   unsigned int sample = analogRead(micPin_);
   if (sample < 1024) {
      if (sample > signalMax_) {
         signalMax_ = sample;
      } else if (sample < signalMin_) {
         signalMin_ = sample;
      }
   }

   if (now - windowStartMs_ < sampleWindowMs_) {
      return;
   }

   unsigned int peakToPeak = signalMax_ - signalMin_;
   Serial.println(peakToPeak);
   if (peakToPeak > threshold_) {
      digitalWrite(motorPin_, HIGH);
   } else {
      digitalWrite(motorPin_, LOW);
   }

   samplingWindowActive_ = false;
}