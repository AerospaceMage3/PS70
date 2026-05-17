#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class SimpleGame {
 public:
  SimpleGame(ILI9341Display& display, Joystick& joystick, Button& button);

  void begin();
  void update();

 private:
  int laneToX(int lane) const;
  void resetRound();
  void spawnEnemy();
  void updatePlayerLane();
  void stepGame();
  void render();

  ILI9341Display& display_;
  Joystick& joystick_;
  Button& button_;

  bool isGameOver_;
  bool previousButtonPressed_;
  unsigned long lastFrameMs_;

  int score_;
  int playerLane_;
  int enemyLane_;
  int enemyY_;
  int previousScore_;
  int previousPlayerLane_;
  int previousEnemyLane_;
  int previousEnemyY_;
  bool previousGameOver_;
  bool needsFullRedraw_;
  unsigned long lastPlayerMoveMs_;

  static constexpr int kLaneCount = 8;
  static constexpr int kTopY = 40;
  static constexpr int kPlayerY = 200;
  static constexpr int kEnemyStepPx = 12;
  static constexpr unsigned long kFrameIntervalMs = 40;
  static constexpr unsigned long kPlayerMoveIntervalMs = 120;
  static constexpr float kMoveThreshold = 0.45f;
};
