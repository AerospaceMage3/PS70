#include "TetrisGame.h"

// ---------------------------------------------------------------------------
// Piece data: 7 tetrominoes × 4 rotations × 4 cells × {dr, dc}
// Pivot is cell index 0 (or centroid); rotations are 0=spawn,1=CW,2=180,3=CCW
// Encoding matches standard Tetris guideline shapes.
// ---------------------------------------------------------------------------
const int8_t TetrisGame::kPieces[7][4][4][2] = {
    // 0: I  (cyan)
    {{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},   // rot0: vertical
     {{0, -1}, {0, 0}, {0, 1}, {0, 2}},    // rot1: horizontal
     {{-1, 0}, {0, 0}, {1, 0}, {2, 0}},   // rot2: same as 0
     {{0, -1}, {0, 0}, {0, 1}, {0, 2}}},   // rot3: same as 1

    // 1: O  (yellow)
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},

    // 2: T  (purple)
    {{{0, -1}, {0, 0}, {0, 1}, {-1, 0}},  // rot0
     {{-1, 0}, {0, 0}, {1, 0}, {0, 1}},   // rot1 CW
     {{0, -1}, {0, 0}, {0, 1}, {1, 0}},   // rot2 180
     {{-1, 0}, {0, 0}, {1, 0}, {0, -1}}}, // rot3 CCW

    // 3: S  (green)
    {{{0, -1}, {0, 0}, {-1, 0}, {-1, 1}},
     {{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
     {{0, -1}, {0, 0}, {-1, 0}, {-1, 1}},
     {{-1, 0}, {0, 0}, {0, 1}, {1, 1}}},

    // 4: Z  (red)
    {{{-1, -1}, {-1, 0}, {0, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {-1, 1}},
     {{-1, -1}, {-1, 0}, {0, 0}, {0, 1}},
     {{0, 0}, {1, 0}, {0, 1}, {-1, 1}}},

    // 5: J  (blue)
    {{{0, -1}, {0, 0}, {0, 1}, {-1, 1}},  // rot0
     {{-1, 0}, {0, 0}, {1, 0}, {1, 1}},   // rot1 CW
     {{0, -1}, {0, 0}, {0, 1}, {1, -1}},  // rot2 180
     {{-1, 0}, {0, 0}, {1, 0}, {-1, -1}}},// rot3 CCW

    // 6: L  (orange)
    {{{0, -1}, {0, 0}, {0, 1}, {-1, -1}}, // rot0
     {{-1, 0}, {0, 0}, {1, 0}, {-1, 1}},  // rot1 CW
     {{0, -1}, {0, 0}, {0, 1}, {1, 1}},   // rot2 180
     {{-1, 0}, {0, 0}, {1, 0}, {1, -1}}}, // rot3 CCW
};

// ILI9341 565-colour approximations
const uint16_t TetrisGame::kColors[7] = {
    0x07FF,  // I: cyan
    0xFFE0,  // O: yellow
    0x801F,  // T: purple  (approximate)
    0x07E0,  // S: green
    0xF800,  // Z: red
    0x001F,  // J: blue
    0xFD20,  // L: orange
};

// ---------------------------------------------------------------------------
TetrisGame::TetrisGame(ILI9341Display& display, Joystick& joystick,
                       Button& button)
    : display_(display),
      joystick_(joystick),
      button_(button),
      pieceType_(0),
      pieceRot_(0),
      pieceRow_(0),
      pieceCol_(0),
      prevPieceType_(-1),
      prevPieceRot_(-1),
      prevPieceRow_(-1),
      prevPieceCol_(-1),
      nextType_(0),
      prevNextType_(-1),
      score_(0),
      lines_(0),
      level_(1),
      prevScore_(-1),
      prevLines_(-1),
      isGameOver_(false),
      prevGameOver_(false),
      needsFullRedraw_(true),
      lastFallMs_(0),
      lastMoveMs_(0),
      lastRotMs_(0),
      lastLockMs_(0),
      prevButtonA_(false),
      prevButtonB_(false),
      prevJoyLeft_(false),
      prevJoyRight_(false),
      prevJoyDown_(false) {
  memset(board_, 0, sizeof(board_));
  memset(prevBoard_, 0xFF, sizeof(prevBoard_));  // force first full draw
}

void TetrisGame::begin() {
  randomSeed(analogRead(A0));
  resetGame();
}

void TetrisGame::resetGame() {
  memset(board_, 0, sizeof(board_));
  memset(prevBoard_, 0xFF, sizeof(prevBoard_));
  score_ = 0;
  lines_ = 0;
  level_ = 1;
  prevScore_ = -1;
  prevLines_ = -1;
  prevNextType_ = -1;
  isGameOver_ = false;
  prevGameOver_ = false;
  needsFullRedraw_ = true;
  lastFallMs_ = millis();
  lastMoveMs_ = millis();
  lastRotMs_ = millis();
  prevButtonA_ = false;
  prevButtonB_ = false;
  prevJoyLeft_ = false;
  prevJoyRight_ = false;
  prevJoyDown_ = false;

  nextType_ = random(0, 7);
  spawnPiece();
  fullRedraw();
}

void TetrisGame::spawnPiece() {
  pieceType_ = nextType_;
  nextType_ = random(0, 7);
  pieceRot_ = 0;
  pieceRow_ = 1;
  pieceCol_ = kCols / 2;

  if (!tryMove(0, 0, 0)) {
    isGameOver_ = true;
  }

  prevPieceType_ = -1;  // force redraw of piece
}

bool TetrisGame::tryMove(int dr, int dc, int rot) const {
  int newRow = pieceRow_ + dr;
  int newCol = pieceCol_ + dc;
  for (int c = 0; c < 4; ++c) {
    int r = newRow + kPieces[pieceType_][rot][c][0];
    int col = newCol + kPieces[pieceType_][rot][c][1];
    if (r < 0 || r >= kRows || col < 0 || col >= kCols) return false;
    if (board_[r][col] != 0) return false;
  }
  return true;
}

void TetrisGame::lockPiece() {
  for (int c = 0; c < 4; ++c) {
    int r = pieceRow_ + kPieces[pieceType_][pieceRot_][c][0];
    int col = pieceCol_ + kPieces[pieceType_][pieceRot_][c][1];
    if (r >= 0 && r < kRows && col >= 0 && col < kCols) {
      board_[r][col] = uint8_t(pieceType_ + 1);
    }
  }
  int cleared = clearLines();
  if (cleared > 0) {
    static const int kScoreTable[5] = {0, 100, 300, 500, 800};
    score_ += kScoreTable[min(cleared, 4)] * level_;
    lines_ += cleared;
    level_ = max(1, lines_ / 10 + 1);
    needsFullRedraw_ = true;  // board changed significantly
  }
  spawnPiece();
}

int TetrisGame::clearLines() {
  int cleared = 0;
  for (int r = kRows - 1; r >= 0;) {
    bool full = true;
    for (int c = 0; c < kCols; ++c) {
      if (board_[r][c] == 0) { full = false; break; }
    }
    if (full) {
      for (int rr = r; rr > 0; --rr) {
        memcpy(board_[rr], board_[rr - 1], kCols);
      }
      memset(board_[0], 0, kCols);
      ++cleared;
      // Don't advance r — recheck same row after shift
    } else {
      --r;
    }
  }
  return cleared;
}

unsigned long TetrisGame::currentFallInterval() const {
  // Each level shaves 40 ms down to a floor of 100 ms
  unsigned long interval = kFallIntervalMs - (level_ - 1) * 40UL;
  return max(interval, 100UL);
}

// ---------------------------------------------------------------------------
void TetrisGame::update() {
  unsigned long now = millis();

  bool btnA = button_.isPressed();
  bool btnAEdge = btnA && !prevButtonA_;
  prevButtonA_ = btnA;

  if (isGameOver_) {
    if (needsFullRedraw_) {
      fullRedraw();
      needsFullRedraw_ = false;
    }
    // Show game-over overlay once
    if (!prevGameOver_) {
      display_.drawText(kBoardX + 2, kBoardY + kRows * kCell / 2 - 10,
                        "GAME OVER", ILI9341_RED, 1, ILI9341_BLACK);
      display_.drawText(kBoardX + 2, kBoardY + kRows * kCell / 2 + 4,
                        "Btn A restart", ILI9341_WHITE, 1, ILI9341_BLACK);
      prevGameOver_ = true;
    }
    if (btnAEdge) {
      resetGame();
    }
    delay(20);
    return;
  }

  Joystick::NormalizedReading joy = joystick_.readNormalized();
  bool joyLeft = joy.y >= kMoveThreshold;
  bool joyRight = joy.y <= -kMoveThreshold;
  bool joyDown = joy.x >= kMoveThreshold;

  bool joyLeftEdge = joyLeft && !prevJoyLeft_;
  bool joyRightEdge = joyRight && !prevJoyRight_;
  bool joyDownEdge = joyDown && !prevJoyDown_;
  prevJoyLeft_ = joyLeft;
  prevJoyRight_ = joyRight;
  prevJoyDown_ = joyDown;

  bool moved = false;

  // Rotate on button A edge
  if (btnAEdge) {
    int newRot = (pieceRot_ + 1) % 4;
    if (tryMove(0, 0, newRot)) {
      pieceRot_ = newRot;
      moved = true;
    } else if (tryMove(0, 1, newRot)) {
      pieceCol_ += 1;
      pieceRot_ = newRot;
      moved = true;
    } else if (tryMove(0, -1, newRot)) {
      pieceCol_ -= 1;
      pieceRot_ = newRot;
      moved = true;
    }
  }

  // Lateral / down movement with repeat throttle
  if (now - lastMoveMs_ >= kMoveIntervalMs) {
    if (joyLeft || joyLeftEdge) {
      if (tryMove(0, -1, pieceRot_)) { pieceCol_--; moved = true; lastMoveMs_ = now; }
    } else if (joyRight || joyRightEdge) {
      if (tryMove(0, 1, pieceRot_)) { pieceCol_++; moved = true; lastMoveMs_ = now; }
    }
    if (joyDown || joyDownEdge) {
      if (tryMove(1, 0, pieceRot_)) { pieceRow_++; moved = true; lastMoveMs_ = now; lastFallMs_ = now; }
    }
  }

  // Auto-fall
  if (now - lastFallMs_ >= currentFallInterval()) {
    lastFallMs_ = now;
    if (tryMove(1, 0, pieceRot_)) {
      pieceRow_++;
      moved = true;
    } else {
      lockPiece();
      render();
      return;
    }
  }

  if (moved) {
    render();
  }
}

// ---------------------------------------------------------------------------
void TetrisGame::render() {
  if (needsFullRedraw_) {
    fullRedraw();
    needsFullRedraw_ = false;
    return;
  }

  // Erase old piece position
  if (prevPieceType_ >= 0) {
    for (int c = 0; c < 4; ++c) {
      int r = prevPieceRow_ + kPieces[prevPieceType_][prevPieceRot_][c][0];
      int col = prevPieceCol_ + kPieces[prevPieceType_][prevPieceRot_][c][1];
      if (r >= 0 && r < kRows && col >= 0 && col < kCols) {
        uint8_t cell = board_[r][col];
        drawCell(r, col, cell == 0 ? ILI9341_BLACK : kColors[cell - 1]);
      }
    }
  }

  // Draw new piece position
  for (int c = 0; c < 4; ++c) {
    int r = pieceRow_ + kPieces[pieceType_][pieceRot_][c][0];
    int col = pieceCol_ + kPieces[pieceType_][pieceRot_][c][1];
    if (r >= 0 && r < kRows && col >= 0 && col < kCols) {
      drawCell(r, col, kColors[pieceType_]);
    }
  }

  prevPieceType_ = pieceType_;
  prevPieceRot_ = pieceRot_;
  prevPieceRow_ = pieceRow_;
  prevPieceCol_ = pieceCol_;

  // HUD update only if changed
  if (score_ != prevScore_ || lines_ != prevLines_) {
    drawHud();
    prevScore_ = score_;
    prevLines_ = lines_;
  }

  if (nextType_ != prevNextType_) {
    drawNextPiece();
    prevNextType_ = nextType_;
  }
}

void TetrisGame::fullRedraw() {
  display_.clear(ILI9341_BLACK);

  // Board border
  display_.drawRect(kBoardX - 1, kBoardY - 1, kCols * kCell + 2,
                    kRows * kCell + 2, ILI9341_WHITE);

  // Locked cells
  for (int r = 0; r < kRows; ++r) {
    for (int c = 0; c < kCols; ++c) {
      uint8_t v = board_[r][c];
      if (v > 0) drawCell(r, c, kColors[v - 1]);
    }
  }

  // Active piece
  for (int c = 0; c < 4; ++c) {
    int r = pieceRow_ + kPieces[pieceType_][pieceRot_][c][0];
    int col = pieceCol_ + kPieces[pieceType_][pieceRot_][c][1];
    if (r >= 0 && r < kRows && col >= 0 && col < kCols) {
      drawCell(r, col, kColors[pieceType_]);
    }
  }

  prevPieceType_ = pieceType_;
  prevPieceRot_ = pieceRot_;
  prevPieceRow_ = pieceRow_;
  prevPieceCol_ = pieceCol_;

  drawHud();
  drawNextPiece();
  memcpy(prevBoard_, board_, sizeof(board_));
  prevScore_ = score_;
  prevLines_ = lines_;
}

void TetrisGame::drawCell(int row, int col, uint16_t color) {
  int x = kBoardX + col * kCell;
  int y = kBoardY + row * kCell;
  display_.fillRect(x + 1, y + 1, kCell - 2, kCell - 2, color);
}

void TetrisGame::drawHud() {
  display_.drawText(kSideX, kBoardY, "TETRIS", ILI9341_CYAN, 1, ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 16, "Score:", ILI9341_WHITE, 1,
                    ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 26, String(score_) + "     ",
                    ILI9341_YELLOW, 1, ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 46, "Lines:", ILI9341_WHITE, 1,
                    ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 56, String(lines_) + "     ",
                    ILI9341_WHITE, 1, ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 76, "Level:", ILI9341_WHITE, 1,
                    ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 86, String(level_) + "     ",
                    ILI9341_GREEN, 1, ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 110, "A:rot", ILI9341_DARKCYAN, 1,
                    ILI9341_BLACK);
  display_.drawText(kSideX, kBoardY + 120, "Joy:mv", ILI9341_DARKCYAN, 1,
                    ILI9341_BLACK);
}

void TetrisGame::drawNextPiece() {
  display_.drawText(kSideX, kBoardY + 140, "Next:", ILI9341_WHITE, 1,
                    ILI9341_BLACK);
  // Clear preview area
  display_.fillRect(kSideX, kBoardY + 152, 4 * kCell, 4 * kCell, ILI9341_BLACK);
  for (int c = 0; c < 4; ++c) {
    int dr = kPieces[nextType_][0][c][0];
    int dc = kPieces[nextType_][0][c][1];
    int x = kSideX + (dc + 1) * kCell;
    int y = kBoardY + 152 + (dr + 1) * kCell;
    display_.fillRect(x + 1, y + 1, kCell - 2, kCell - 2, kColors[nextType_]);
  }
}
