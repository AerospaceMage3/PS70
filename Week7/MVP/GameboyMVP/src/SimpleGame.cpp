#include "SimpleGame.h"

SimpleGame::SimpleGame(ILI9341Display& display, Joystick& joystick, Button& button)
    : display_(display),
      joystick_(joystick),
      button_(button),
      isGameOver_(false),
      previousButtonPressed_(false),
      lastFrameMs_(0),
      score_(0),
      playerLane_(kLaneCount / 2),
      enemyLane_(0),
      enemyY_(kTopY),
      previousScore_(-1),
      previousPlayerLane_(kLaneCount / 2),
      previousEnemyLane_(0),
      previousEnemyY_(kTopY),
      previousGameOver_(false),
      needsFullRedraw_(true),
      lastPlayerMoveMs_(0) {}

void SimpleGame::begin() {
  randomSeed(analogRead(A0));
  resetRound();
  render();
}

void SimpleGame::update() {
  bool buttonPressed = button_.isPressed();
  bool buttonPressedEdge = buttonPressed && !previousButtonPressed_;
  previousButtonPressed_ = buttonPressed;

  if (isGameOver_) {
    if (buttonPressedEdge) {
      resetRound();
    }
    render();
    delay(20);
    return;
  }

  unsigned long now = millis();
  if (now - lastFrameMs_ < kFrameIntervalMs) {
    return;
  }
  lastFrameMs_ = now;

  updatePlayerLane();
  stepGame();
  render();
}

int SimpleGame::laneToX(int lane) const {
  int safeLane = constrain(lane, 0, kLaneCount - 1);
  int left = 20;
  int right = max(display_.width() - 20, left + 1);
  int span = right - left;
  return left + (span * safeLane) / (kLaneCount - 1);
}

void SimpleGame::resetRound() {
  isGameOver_ = false;
  score_ = 0;
  playerLane_ = kLaneCount / 2;
  spawnEnemy();
  enemyY_ = kTopY;
  previousScore_ = -1;
  previousPlayerLane_ = playerLane_;
  previousEnemyLane_ = enemyLane_;
  previousEnemyY_ = enemyY_;
  previousGameOver_ = false;
  needsFullRedraw_ = true;
  lastFrameMs_ = millis();
  lastPlayerMoveMs_ = 0;
}

void SimpleGame::spawnEnemy() {
  enemyLane_ = random(0, kLaneCount);
}

void SimpleGame::updatePlayerLane() {
  Joystick::NormalizedReading norm = joystick_.readNormalized();
  float normalizedX = constrain(norm.x, -1.0f, 1.0f);
  unsigned long now = millis();

  if (normalizedX <= -kMoveThreshold) {
    if (now - lastPlayerMoveMs_ >= kPlayerMoveIntervalMs) {
      playerLane_ = max(0, playerLane_ - 1);
      lastPlayerMoveMs_ = now;
    }
    return;
  }

  if (normalizedX >= kMoveThreshold) {
    if (now - lastPlayerMoveMs_ >= kPlayerMoveIntervalMs) {
      playerLane_ = min(kLaneCount - 1, playerLane_ + 1);
      lastPlayerMoveMs_ = now;
    }
  }
}

void SimpleGame::stepGame() {
  enemyY_ += kEnemyStepPx;

  if (enemyY_ >= kPlayerY) {
    if (enemyLane_ == playerLane_) {
      isGameOver_ = true;
      return;
    }

    score_++;
    enemyY_ = kTopY;
    spawnEnemy();
  }
}

void SimpleGame::render() {
  if (needsFullRedraw_) {
    display_.clear(ILI9341_BLACK);
    display_.drawText(10, 8, "DODGE", ILI9341_CYAN, 2, ILI9341_BLACK);
    display_.drawText(10, 28, "Score: " + String(score_) + "   ", ILI9341_WHITE,
                      2, ILI9341_BLACK);
    needsFullRedraw_ = false;
  } else {
    int previousEnemyX = laneToX(previousEnemyLane_);
    int previousPlayerX = laneToX(previousPlayerLane_);
    display_.drawText(previousEnemyX, previousEnemyY_, "O", ILI9341_BLACK, 2,
                      ILI9341_BLACK);
    display_.drawText(previousPlayerX, kPlayerY, "A", ILI9341_BLACK, 2,
                      ILI9341_BLACK);

    if (score_ != previousScore_) {
      display_.drawText(10, 28, "Score: " + String(score_) + "   ", ILI9341_WHITE,
                        2, ILI9341_BLACK);
    }
  }

  int enemyX = laneToX(enemyLane_);
  int playerX = laneToX(playerLane_);
  display_.drawText(enemyX, enemyY_, "O", ILI9341_RED, 2, ILI9341_BLACK);
  display_.drawText(playerX, kPlayerY, "A", ILI9341_GREEN, 2, ILI9341_BLACK);

  if (isGameOver_ && !previousGameOver_) {
    display_.drawText(10, 130, "GAME OVER", ILI9341_YELLOW, 2, ILI9341_BLACK);
    display_.drawText(10, 155, "Press Btn", ILI9341_WHITE, 2, ILI9341_BLACK);
  }

  if (!isGameOver_ && previousGameOver_) {
    display_.drawText(10, 130, "GAME OVER", ILI9341_BLACK, 2, ILI9341_BLACK);
    display_.drawText(10, 155, "Press Btn", ILI9341_BLACK, 2, ILI9341_BLACK);
  }

  previousEnemyLane_ = enemyLane_;
  previousEnemyY_ = enemyY_;
  previousPlayerLane_ = playerLane_;
  previousScore_ = score_;
  previousGameOver_ = isGameOver_;
}
