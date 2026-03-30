#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"
#include "SimpleGame.h"
#include "SnakeGame.h"

constexpr uint8_t JOYSTICK_X_PIN = A0;
constexpr uint8_t JOYSTICK_Y_PIN = A1;
constexpr uint8_t CONTROLLER_BUTTON_PIN = D7;
constexpr uint8_t DISPLAY_SCK_PIN = D8;
constexpr uint8_t DISPLAY_MOSI_PIN = D10;
constexpr uint8_t DISPLAY_LED_PIN = D4;
constexpr uint8_t DISPLAY_DC_PIN = D5;
constexpr uint8_t DISPLAY_CS_PIN = D6;
constexpr uint8_t DISPLAY_RST_PIN = D3;

Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, 4095, 50);
Button controllerButton(CONTROLLER_BUTTON_PIN);
ILI9341Display display(DISPLAY_SCK_PIN, DISPLAY_MOSI_PIN, DISPLAY_LED_PIN,
                       DISPLAY_DC_PIN, DISPLAY_CS_PIN, DISPLAY_RST_PIN);
SnakeGame snakeGame(display, joystick, controllerButton);
SimpleGame dodgeGame(display, joystick, controllerButton);

enum class AppMode {
  Menu,
  Snake,
  Dodge,
};

constexpr unsigned long TRIPLE_TAP_WINDOW_MS = 3000;
constexpr unsigned long MENU_MOVE_INTERVAL_MS = 180;
constexpr float MENU_MOVE_THRESHOLD = 0.55f;

AppMode appMode = AppMode::Menu;
int menuIndex = 0;
bool menuDirty = true;
bool previousButtonPressed = false;
unsigned long tapTimes[3] = {0, 0, 0};
uint8_t tapCount = 0;
unsigned long lastMenuMoveMs = 0;

void renderMenu() {
  if (!menuDirty) {
    return;
  }

  display.clear(ILI9341_BLACK);
  display.drawText(16, 20, "MAIN MENU", ILI9341_CYAN, 2, ILI9341_BLACK);
  display.drawText(16, 60, "Choose Game:", ILI9341_WHITE, 2, ILI9341_BLACK);

  uint16_t snakeColor = (menuIndex == 0) ? ILI9341_YELLOW : ILI9341_WHITE;
  uint16_t dodgeColor = (menuIndex == 1) ? ILI9341_YELLOW : ILI9341_WHITE;
  String snakePrefix = (menuIndex == 0) ? "> " : "  ";
  String dodgePrefix = (menuIndex == 1) ? "> " : "  ";

  display.drawText(16, 100, snakePrefix + "Snake", snakeColor, 2, ILI9341_BLACK);
  display.drawText(16, 130, dodgePrefix + "Dodge", dodgeColor, 2, ILI9341_BLACK);

  display.drawText(16, 180, "Move: Joy X", ILI9341_DARKCYAN, 1, ILI9341_BLACK);
  display.drawText(16, 196, "Select: Btn tap", ILI9341_DARKCYAN, 1,
                   ILI9341_BLACK);
  display.drawText(16, 212, "3 taps: Menu", ILI9341_DARKCYAN, 1,
                   ILI9341_BLACK);

  menuDirty = false;
}

void startSelectedGame() {
  if (menuIndex == 0) {
    appMode = AppMode::Snake;
    snakeGame.begin();
    return;
  }

  appMode = AppMode::Dodge;
  dodgeGame.begin();
}

void enterMenu() {
  appMode = AppMode::Menu;
  menuDirty = true;
}

bool processButtonTaps() {
  bool pressed = controllerButton.isPressed();
  bool pressedEdge = pressed && !previousButtonPressed;
  previousButtonPressed = pressed;

  if (!pressedEdge) {
    if (tapCount > 0 && millis() - tapTimes[tapCount - 1] > TRIPLE_TAP_WINDOW_MS) {
      tapCount = 0;
    }
    return false;
  }

  unsigned long now = millis();
  if (tapCount > 0 && now - tapTimes[tapCount - 1] > TRIPLE_TAP_WINDOW_MS) {
    tapCount = 0;
  }

  if (tapCount < 3) {
    tapTimes[tapCount++] = now;
  } else {
    tapTimes[0] = tapTimes[1];
    tapTimes[1] = tapTimes[2];
    tapTimes[2] = now;
    tapCount = 3;
  }

  if (tapCount == 3 && (now - tapTimes[0] <= TRIPLE_TAP_WINDOW_MS)) {
    tapCount = 0;
    enterMenu();
    return true;
  }

  if (appMode == AppMode::Menu) {
    startSelectedGame();
    tapCount = 0;
    return true;
  }

  return false;
}

void setup() {
  Serial.begin(115200);
  joystick.begin();
  controllerButton.begin();
  display.begin();

  delay(250);
  joystick.calibrateCenter();
  renderMenu();
}

void loop() {
  if (processButtonTaps()) {
    renderMenu();
    return;
  }

  if (appMode == AppMode::Menu) {
    unsigned long now = millis();
    if (now - lastMenuMoveMs >= MENU_MOVE_INTERVAL_MS) {
      Joystick::NormalizedReading menuInput = joystick.readNormalized();
      int previousIndex = menuIndex;

      if (menuInput.x <= -MENU_MOVE_THRESHOLD) {
        menuIndex = max(0, menuIndex - 1);
        lastMenuMoveMs = now;
      } else if (menuInput.x >= MENU_MOVE_THRESHOLD) {
        menuIndex = min(1, menuIndex + 1);
        lastMenuMoveMs = now;
      }

      if (menuIndex != previousIndex) {
        menuDirty = true;
      }
    }

    renderMenu();
    delay(20);
    return;
  }

  if (appMode == AppMode::Snake) {
    snakeGame.update();
  } else {
    dodgeGame.update();
  }
}