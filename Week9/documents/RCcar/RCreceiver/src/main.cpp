// Jacob ESP32 RC Car receiver sketch
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include "DriveMotor.h"
#include "SteeringServo.h"

// Sender Mac Address
// 0x24, 0xEC, 0x4A, 0xCE, 0x14, 0x78

constexpr uint8_t ESPNOW_CHANNEL = 1;
constexpr uint8_t STEERING_SERVO_PIN = D0;
constexpr uint8_t DRIVE_FORWARD_PIN = D1;
constexpr uint8_t DRIVE_BACKWARD_PIN = D2;


// Must match the sender struct exactly
typedef struct struct_message {
  int x;
  int y;
} struct_message;

struct_message receivedData;
volatile bool newDataReceived = false;
unsigned long lastPacketMs = 0;
unsigned long lastStatusMs = 0;
const unsigned long statusIntervalMs = 3000;
SteeringServo steeringServo(STEERING_SERVO_PIN);
DriveMotor driveMotor(DRIVE_FORWARD_PIN, DRIVE_BACKWARD_PIN);

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len != sizeof(receivedData)) {
    Serial.print("Unexpected packet size: ");
    Serial.println(len);
    return;
  }

  memcpy(&receivedData, incomingData, sizeof(receivedData));
  lastPacketMs = millis();
  newDataReceived = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting RC car receiver...");

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.print("Receiver MAC address: ");
  Serial.println(WiFi.macAddress());

  if (esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE) != ESP_OK) {
    Serial.println("Failed to set Wi-Fi channel");
    return;
  }
  Serial.print("ESP-NOW channel: ");
  Serial.println(ESPNOW_CHANNEL);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  steeringServo.begin();
  driveMotor.begin();
  Serial.print("Steering servo initial angle: ");
  Serial.println(steeringServo.currentAngle());
  Serial.println("Receiver ready");
}

void loop() {
  if (newDataReceived) {
    newDataReceived = false;
    lastStatusMs = millis();

    Serial.print("Received X: ");
    Serial.print(receivedData.x);
    Serial.print("  Received Y: ");
    Serial.print(receivedData.y);

    steeringServo.updateFromDiscreteInput(receivedData.x);
    driveMotor.updateFromDiscreteInput(receivedData.y);
    Serial.print("  Steering angle: ");
    Serial.println(steeringServo.currentAngle());
  } else if (millis() - lastStatusMs >= statusIntervalMs) {
    lastStatusMs = millis();

    if (lastPacketMs == 0) {
      Serial.println("Waiting for sender...");

    } else {
      Serial.print("No new packet for ");
      Serial.print(millis() - lastPacketMs);
      Serial.println(" ms");
    }
  }

  delay(50);
}