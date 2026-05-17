#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class TetrisGame {
 public:
  TetrisGame(ILI9341Display& display, Joystick& joystick, Button& button);

  void begin();
  void update();

 private:
  // Board dimensions
  static constexpr int kCols = 10;
  static constexpr int kRows = 20;
  static constexpr int kCell = 11;  // pixels per cell

  // Playfield top-left pixel
  static constexpr int kBoardX = 5;
  static constexpr int kBoardY = 10;

  // Sidebar
  static constexpr int kSideX = kBoardX + kCols * kCell + 6;

  // Timing
  static constexpr unsigned long kFallIntervalMs = 500;
  static constexpr unsigned long kMoveIntervalMs = 130;
  static constexpr unsigned long kLockDelayMs = 500;

  // Input thresholds
  static constexpr float kMoveThreshold = 0.50f;

  // 7 standard tetrominoes, 4 rotations each, encoded as 4 (row,col) offsets
  // relative to pivot, stored flat: piece[type][rot][cell*2] → {dr, dc}
  static const int8_t kPieces[7][4][4][2];

  // Colours for each piece type (index 0–6 = I,O,T,S,Z,J,L)
  static const uint16_t kColors[7];

  // Board: 0 = empty, 1–7 = locked piece color index
  uint8_t board_[kRows][kCols];

  // Previous board snapshot for incremental redraws
  uint8_t prevBoard_[kRows][kCols];

  // Active piece state
  int pieceType_;
  int pieceRot_;
  int pieceRow_;  // pivot row
  int pieceCol_;  // pivot col
  int prevPieceType_;
  int prevPieceRot_;
  int prevPieceRow_;
  int prevPieceCol_;

  // Next piece
  int nextType_;
  int prevNextType_;

  int score_;
  int lines_;
  int level_;
  int prevScore_;
  int prevLines_;
  bool isGameOver_;
  bool prevGameOver_;
  bool needsFullRedraw_;

  // Timing
  unsigned long lastFallMs_;
  unsigned long lastMoveMs_;
  unsigned long lastRotMs_;
  unsigned long lastLockMs_;

  // Input edge tracking
  bool prevButtonA_;
  bool prevButtonB_;
  bool prevJoyLeft_;
  bool prevJoyRight_;
  bool prevJoyDown_;

  ILI9341Display& display_;
  Joystick& joystick_;
  Button& button_;

  void resetGame();
  void spawnPiece();
  bool tryMove(int dr, int dc, int rot) const;
  void lockPiece();
  int clearLines();
  void render();
  void fullRedraw();
  void drawBoard();
  void drawCell(int row, int col, uint16_t color);
  void drawActivePiece(uint16_t color);
  void drawNextPiece();
  void drawHud();
  unsigned long currentFallInterval() const;
};
