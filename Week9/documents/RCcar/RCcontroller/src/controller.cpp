// Jacob ESP 32 RC Controller sketch
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include "Joystick.h"

// Broadcast MAC address (all peers on the same ESP-NOW channel receive this)
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
constexpr uint8_t ESPNOW_CHANNEL = 1;

constexpr uint8_t JOYSTICK_X_PIN = D0;
constexpr uint8_t JOYSTICK_Y_PIN = D1;

Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int x;
  int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  joystick.begin();
  Serial.println("Calibrating joystick center... keep it untouched");
  joystick.calibrateCenter();
  Serial.print("Calibrated center X: ");
  Serial.print(joystick.centerX());
  Serial.print("  Center Y: ");
  Serial.println(joystick.centerY());
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.print("Controller MAC address: ");
  Serial.println(WiFi.macAddress());

  if (esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE) != ESP_OK) {
    Serial.println("Failed to set Wi-Fi channel");
    return;
  }
  Serial.print("ESP-NOW channel: ");
  Serial.println(ESPNOW_CHANNEL);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = ESPNOW_CHANNEL;
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  int rawX = joystick.readX();
  int rawY = joystick.readY();

  // Send discrete direction states (-1, 0, 1)
  myData.x = joystick.axisStateX(rawX);
  myData.y = joystick.axisStateY(rawY);

  // Debug output
  Serial.print("Raw X: ");
  Serial.print(rawX);
  Serial.print("  Raw Y: ");
  Serial.print(rawY);
  Serial.print("  Sent X: ");
  Serial.print(myData.x);
  Serial.print("  Sent Y: ");
  Serial.println(myData.y);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(500);
}