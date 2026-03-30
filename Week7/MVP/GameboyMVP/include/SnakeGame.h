#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class SnakeGame {
 public:
  SnakeGame(ILI9341Display& display, Joystick& joystick, Button& button);

  void begin();
  void update();

 private:
  struct Cell {
    int x;
    int y;
  };

  void drawFrame();
  void drawScore();
  void drawCell(const Cell& cell, uint16_t color);
  int cellToPixelX(int cellX) const;
  int cellToPixelY(int cellY) const;
  void resetGame();
  void spawnFood();
  void updateDirectionFromJoystick();
  void stepSnake();
  void render();
  bool isCellOccupiedBySnake(int x, int y) const;

  ILI9341Display& display_;
  Joystick& joystick_;
  Button& button_;

  static constexpr int kGridWidth = 16;
  static constexpr int kGridHeight = 12;
  static constexpr int kMaxSnakeLength = kGridWidth * kGridHeight;
  static constexpr unsigned long kStepIntervalMs = 110;
  static constexpr float kTurnThreshold = 0.45f;
  static constexpr int kCellSize = 12;
  static constexpr int kPlayfieldX = 8;
  static constexpr int kPlayfieldY = 28;
  static constexpr int kCellPadding = 1;

  Cell snake_[kMaxSnakeLength];
  int snakeLength_;
  int dirX_;
  int dirY_;
  Cell food_;
  int score_;
  bool isGameOver_;
  bool previousButtonPressed_;
  unsigned long lastStepMs_;
  Cell lastTail_;
  bool tailClearedThisStep_;
  bool needsFullRedraw_;
};
