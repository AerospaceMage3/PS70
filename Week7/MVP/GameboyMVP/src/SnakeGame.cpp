#include "SnakeGame.h"

SnakeGame::SnakeGame(ILI9341Display& display, Joystick& joystick, Button& button)
    : display_(display),
      joystick_(joystick),
      button_(button),
      snakeLength_(0),
      dirX_(1),
      dirY_(0),
      food_{0, 0},
      score_(0),
      isGameOver_(false),
      previousButtonPressed_(false),
      lastStepMs_(0),
      lastTail_{0, 0},
      tailClearedThisStep_(false),
      needsFullRedraw_(true) {}

void SnakeGame::begin() {
  randomSeed(analogRead(A0));
  resetGame();
  render();
}

void SnakeGame::update() {
  bool buttonPressed = button_.isPressed();
  bool buttonPressedEdge = buttonPressed && !previousButtonPressed_;
  previousButtonPressed_ = buttonPressed;

  if (isGameOver_) {
    if (buttonPressedEdge) {
      resetGame();
    }
    render();
    delay(20);
    return;
  }

  updateDirectionFromJoystick();

  unsigned long now = millis();
  if (now - lastStepMs_ < kStepIntervalMs) {
    return;
  }
  lastStepMs_ = now;

  stepSnake();
  render();
}

void SnakeGame::resetGame() {
  snakeLength_ = 3;
  score_ = 0;
  isGameOver_ = false;

  int centerX = kGridWidth / 2;
  int centerY = kGridHeight / 2;

  snake_[0] = {centerX, centerY};
  snake_[1] = {centerX - 1, centerY};
  snake_[2] = {centerX - 2, centerY};

  dirX_ = 1;
  dirY_ = 0;

  spawnFood();
  tailClearedThisStep_ = false;
  needsFullRedraw_ = true;
  lastStepMs_ = millis();
}

void SnakeGame::spawnFood() {
  while (true) {
    int foodX = random(0, kGridWidth);
    int foodY = random(0, kGridHeight);
    if (!isCellOccupiedBySnake(foodX, foodY)) {
      food_ = {foodX, foodY};
      return;
    }
  }
}

void SnakeGame::updateDirectionFromJoystick() {
  Joystick::NormalizedReading reading = joystick_.readNormalized();

  if (reading.y <= -kTurnThreshold && dirX_ != -1) {
    dirX_ = 1;
    dirY_ = 0;
    return;
  }

  if (reading.y >= kTurnThreshold && dirX_ != 1) {
    dirX_ = -1;
    dirY_ = 0;
    return;
  }

  if (reading.x <= -kTurnThreshold && dirY_ != 1) {
    dirX_ = 0;
    dirY_ = -1;
    return;
  }

  if (reading.x >= kTurnThreshold && dirY_ != -1) {
    dirX_ = 0;
    dirY_ = 1;
  }
}

void SnakeGame::stepSnake() {
  Cell nextHead = {snake_[0].x + dirX_, snake_[0].y + dirY_};

  if (nextHead.x < 0 || nextHead.x >= kGridWidth || nextHead.y < 0 ||
      nextHead.y >= kGridHeight) {
    isGameOver_ = true;
    return;
  }

  for (int i = 0; i < snakeLength_; ++i) {
    if (snake_[i].x == nextHead.x && snake_[i].y == nextHead.y) {
      isGameOver_ = true;
      return;
    }
  }

  bool ateFood = (nextHead.x == food_.x && nextHead.y == food_.y);

  int newLength = snakeLength_;
  Cell previousTail = snake_[snakeLength_ - 1];
  if (ateFood && snakeLength_ < kMaxSnakeLength) {
    newLength++;
    score_++;
  }

  for (int i = newLength - 1; i > 0; --i) {
    snake_[i] = snake_[i - 1];
  }
  snake_[0] = nextHead;
  snakeLength_ = newLength;
  lastTail_ = previousTail;
  tailClearedThisStep_ = !ateFood;

  if (ateFood) {
    if (snakeLength_ >= kMaxSnakeLength) {
      isGameOver_ = true;
      return;
    }
    spawnFood();
  }
}

void SnakeGame::render() {
  if (needsFullRedraw_) {
    drawFrame();
    drawScore();
    drawCell(food_, ILI9341_RED);
    for (int i = snakeLength_ - 1; i >= 0; --i) {
      drawCell(snake_[i], (i == 0) ? ILI9341_GREEN : ILI9341_DARKGREEN);
    }
    needsFullRedraw_ = false;
  } else {
    if (tailClearedThisStep_) {
      drawCell(lastTail_, ILI9341_BLACK);
    }

    if (snakeLength_ > 1) {
      drawCell(snake_[1], ILI9341_DARKGREEN);
    }
    drawCell(snake_[0], ILI9341_GREEN);
    drawCell(food_, ILI9341_RED);
    drawScore();
  }

  if (isGameOver_) {
    display_.drawText(18, 220, "GAME OVER - Btn Restart", ILI9341_YELLOW, 1,
                      ILI9341_BLACK);
  } else {
    display_.drawText(18, 220, "GAME OVER - Btn Restart", ILI9341_BLACK, 1,
                      ILI9341_BLACK);
  }
}

void SnakeGame::drawFrame() {
  display_.clear(ILI9341_BLACK);
  int borderWidth = kGridWidth * kCellSize + 2;
  int borderHeight = kGridHeight * kCellSize + 2;
  display_.drawRect(kPlayfieldX - 1, kPlayfieldY - 1, borderWidth, borderHeight,
                    ILI9341_DARKCYAN);
}

void SnakeGame::drawScore() {
  display_.drawText(8, 6, "SNAKE", ILI9341_CYAN, 2, ILI9341_BLACK);
  display_.drawText(120, 6, "Score:" + String(score_) + "   ", ILI9341_WHITE, 2,
                    ILI9341_BLACK);
}

void SnakeGame::drawCell(const Cell& cell, uint16_t color) {
  int x = cellToPixelX(cell.x);
  int y = cellToPixelY(cell.y);
  int size = kCellSize - (2 * kCellPadding);
  display_.fillRect(x + kCellPadding, y + kCellPadding, size, size, color);
}

int SnakeGame::cellToPixelX(int cellX) const {
  return kPlayfieldX + cellX * kCellSize;
}

int SnakeGame::cellToPixelY(int cellY) const {
  return kPlayfieldY + cellY * kCellSize;
}

bool SnakeGame::isCellOccupiedBySnake(int x, int y) const {
  for (int i = 0; i < snakeLength_; ++i) {
    if (snake_[i].x == x && snake_[i].y == y) {
      return true;
    }
  }
  return false;
}
