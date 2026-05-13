#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"
#include "SimpleGame.h"
#include "SnakeGame.h"
#include "TetrisGame.h"
#include "TankyGame.h"
#include "Wolf3DGame.h"
// #define ENABLE_PS70_GAME
#ifdef ENABLE_PS70_GAME
#include "PS70Game.h"
#endif

constexpr uint8_t JOYSTICK_X_PIN = A0;
constexpr uint8_t JOYSTICK_Y_PIN = A1;
constexpr uint8_t CONTROLLER_BUTTON_A_PIN = D7;
constexpr uint8_t CONTROLLER_BUTTON_B_PIN = D2;
constexpr uint8_t DISPLAY_SCK_PIN = D8;
constexpr uint8_t DISPLAY_MOSI_PIN = D10;
constexpr uint8_t DISPLAY_LED_PIN = D4;
constexpr uint8_t DISPLAY_DC_PIN = D5;
constexpr uint8_t DISPLAY_CS_PIN = D6;
constexpr uint8_t DISPLAY_RST_PIN = D3;

Joystick joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, 4095, 50);
Button controllerButtonA(CONTROLLER_BUTTON_A_PIN);
Button controllerButtonB(CONTROLLER_BUTTON_B_PIN);
ILI9341Display display(DISPLAY_SCK_PIN, DISPLAY_MOSI_PIN, DISPLAY_LED_PIN,
                       DISPLAY_DC_PIN, DISPLAY_CS_PIN, DISPLAY_RST_PIN);
SnakeGame snakeGame(display, joystick, controllerButtonA);
SimpleGame dodgeGame(display, joystick, controllerButtonA);
TetrisGame tetrisGame(display, joystick, controllerButtonA);
TankyGame tankyGame(display, joystick, controllerButtonA);
Wolf3DGame wolf3dGame(display, joystick, controllerButtonA);
#ifdef ENABLE_PS70_GAME
PS70Game ps70Game(display, joystick, controllerButtonA);
#endif

enum class AppMode {
  Menu,
  Snake,
  Dodge,
  Tetris,
  Tanky,
  Wolf3D,
#ifdef ENABLE_PS70_GAME
  PS70,
#endif
};

constexpr unsigned long MENU_MOVE_INTERVAL_MS = 180;
constexpr float MENU_MOVE_THRESHOLD = 0.55f;

AppMode appMode = AppMode::Menu;
int menuIndex = 0;
bool menuDirty = true;
bool previousButtonPressed = false;
bool previousButtonBPressed = false;
unsigned long lastMenuMoveMs = 0;

void renderMenu() {
  if (!menuDirty) {
    return;
  }

  display.clear(ILI9341_BLACK);
  display.drawText(16, 20, "MAIN MENU", ILI9341_CYAN, 2, ILI9341_BLACK);
  display.drawText(16, 60, "Choose Game:", ILI9341_WHITE, 2, ILI9341_BLACK);

#ifdef ENABLE_PS70_GAME
  const char* names[6] = {"Snake", "Dodge", "Tetris", "Tanky", "DOOM?", "PS70 Lab"};
  const int numGames = 6;
#else
  const char* names[5] = {"Snake", "Dodge", "Tetris", "Tanky", "DOOM?"};
  const int numGames = 5;
#endif
  for (int i = 0; i < numGames; ++i) {
    uint16_t color = (menuIndex == i) ? ILI9341_YELLOW : ILI9341_WHITE;
    String label = (menuIndex == i) ? "> " : "  ";
    label += names[i];
    display.drawText(16, 90 + i * 22, label, color, 1, ILI9341_BLACK);
  }

  display.drawText(16, 210, "Move: Joy X", ILI9341_DARKCYAN, 1, ILI9341_BLACK);
  display.drawText(16, 222, "Select: Btn tap", ILI9341_DARKCYAN, 1,
                   ILI9341_BLACK);

  menuDirty = false;
}

void startSelectedGame() {
  if (menuIndex == 0) {
    appMode = AppMode::Snake;
    snakeGame.begin();
  } else if (menuIndex == 1) {
    appMode = AppMode::Dodge;
    dodgeGame.begin();
  } else if (menuIndex == 2) {
    appMode = AppMode::Tetris;
    tetrisGame.begin();
  } else if (menuIndex == 3) {
    appMode = AppMode::Tanky;
    tankyGame.begin();
  } else if (menuIndex == 4) {
    appMode = AppMode::Wolf3D;
    wolf3dGame.begin();
#ifdef ENABLE_PS70_GAME
  } else {
    appMode = AppMode::PS70;
    ps70Game.begin();
#endif
  }
}

void enterMenu() {
  appMode = AppMode::Menu;
  menuDirty = true;
}

// Returns true if a menu-level event was handled (home press or game launch).
bool processButtons() {
  bool homePressed = controllerButtonB.isPressed();
  bool homeEdge = homePressed && !previousButtonBPressed;
  previousButtonBPressed = homePressed;

  if (homeEdge && appMode != AppMode::Menu) {
    enterMenu();
    return true;
  }

  bool pressed = controllerButtonA.isPressed();
  bool pressedEdge = pressed && !previousButtonPressed;
  previousButtonPressed = pressed;

  if (pressedEdge && appMode == AppMode::Menu) {
    startSelectedGame();
    return true;
  }

  return false;
}

// 16-bit RGB565 color from 8-bit components
static constexpr uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) |
         ((uint16_t)(g & 0xFC) << 3) |
         (b >> 3);
}

void drawStartupAnimation() {
  const int16_t W = display.width();   // 288
  const int16_t H = display.height();  // 240

  // --- Phase 1: scanline wipe from top to bottom ---
  for (int16_t y = 0; y < H; y += 4) {
    // Leading edge: bright cyan bar
    display.fillRect(0, y, W, 4, ILI9341_CYAN);
    // Trail behind it fades to dark teal
    if (y > 12) {
      display.fillRect(0, y - 12, W, 8, rgb565(0, 48, 64));
    }
    if (y > 4) {
      display.fillRect(0, y - 4,  W, 4, rgb565(0, 80, 100));
    }
    delay(6);
  }
  // Settle to solid deep background
  display.fillRect(0, 0, W, H, rgb565(0, 8, 20));

  // --- Phase 2: title "GAMEMAN" materializes letter by letter ---
  const char* title = "GAMEMAN";
  const int16_t titleSize = 4;        // 4× scale = 24px tall chars
  const int16_t charW = 6 * titleSize; // GFX font: 6px wide per char at scale
  const int16_t titleLen = 7;
  const int16_t titleTotalW = titleLen * charW;
  const int16_t titleX = (W - titleTotalW) / 2;
  const int16_t titleY = 72;

  for (int i = 0; i < titleLen; i++) {
    char buf[2] = {title[i], '\0'};
    // Flash highlight then settle to white
    display.drawText(titleX + i * charW, titleY, buf, ILI9341_CYAN, titleSize,
                     rgb565(0, 8, 20));
    delay(60);
    display.drawText(titleX + i * charW, titleY, buf, ILI9341_WHITE, titleSize,
                     rgb565(0, 8, 20));
  }

  delay(120);

  // --- Phase 3: glowing underline sweeps in ---
  const int16_t ulY = titleY + titleSize * 8 + 4;
  for (int16_t x = titleX; x < titleX + titleTotalW; x += 6) {
    display.fillRect(x, ulY, 6, 3, ILI9341_CYAN);
    delay(12);
  }

  // --- Phase 4: subtext drops in from below ---
  const char* sub = "A PS70 Project";
  const int16_t subSize = 2;
  const int16_t subLen = 14;
  const int16_t subW = subLen * 6 * subSize;
  const int16_t subX = (W - subW) / 2;
  const int16_t subTargetY = ulY + 24;
  const int16_t subStartY  = H + 10;

  // Slide up
  for (int16_t y = subStartY; y > subTargetY; y -= 6) {
    display.fillRect(subX, max((int16_t)(y - 6), subTargetY), subW,
                     subSize * 8 + 12, rgb565(0, 8, 20));
    display.drawText(subX, y, sub, rgb565(160, 200, 220), subSize,
                     rgb565(0, 8, 20));
    delay(14);
  }
  display.drawText(subX, subTargetY, sub, rgb565(160, 200, 220), subSize,
                   rgb565(0, 8, 20));

  // --- Phase 5: border sparkle pulse (3 flashes) ---
  for (int flash = 0; flash < 3; flash++) {
    uint16_t col = (flash % 2 == 0) ? ILI9341_CYAN : rgb565(80, 160, 255);
    display.drawRect(2, 2, W - 4, H - 4, col);
    display.drawRect(4, 4, W - 8, H - 8, col);
    delay(120);
    display.drawRect(2, 2, W - 4, H - 4, rgb565(0, 8, 20));
    display.drawRect(4, 4, W - 8, H - 8, rgb565(0, 8, 20));
    delay(80);
  }

  // --- Phase 6: "Press A to Start" prompt blinks in ---
  const char* prompt = "Press A to Start";
  const int16_t promptSize = 1;
  const int16_t promptW = 16 * 6 * promptSize;
  const int16_t promptX = (W - promptW) / 2;
  const int16_t promptY = subTargetY + 48;

  display.drawText(promptX, promptY, prompt, ILI9341_YELLOW, promptSize,
                   rgb565(0, 8, 20));

  // Wait for Button A press (or 4s timeout), blinking the prompt
  unsigned long deadline = millis() + 4000;
  bool visible = true;
  unsigned long lastBlink = millis();
  bool prevA = controllerButtonA.isPressed();

  while (millis() < deadline) {
    bool nowA = controllerButtonA.isPressed();
    if (nowA && !prevA) break;
    prevA = nowA;

    if (millis() - lastBlink >= 400) {
      visible = !visible;
      uint16_t c = visible ? ILI9341_YELLOW : rgb565(0, 8, 20);
      display.drawText(promptX, promptY, prompt, c, promptSize, rgb565(0, 8, 20));
      lastBlink = millis();
    }
    delay(20);
  }

  // Brief white flash before handing off to menu
  display.fillRect(0, 0, W, H, ILI9341_WHITE);
  delay(60);
}

void setup() {
  Serial.begin(115200);
  joystick.begin();
  controllerButtonA.begin();
  controllerButtonB.begin();
  display.begin();

  delay(250);
  joystick.calibrateCenter();

  drawStartupAnimation();

  renderMenu();
}

void loop() {
  if (processButtons()) {
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
#ifdef ENABLE_PS70_GAME
        menuIndex = min(5, menuIndex + 1);
#else
        menuIndex = min(4, menuIndex + 1);
#endif
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
  } else if (appMode == AppMode::Dodge) {
    dodgeGame.update();
  } else if (appMode == AppMode::Tetris) {
    tetrisGame.update();
  } else if (appMode == AppMode::Tanky) {
    tankyGame.update();
  } else if (appMode == AppMode::Wolf3D) {
    wolf3dGame.update();
#ifdef ENABLE_PS70_GAME
  } else if (appMode == AppMode::PS70) {
    ps70Game.update();
#endif
  }
}