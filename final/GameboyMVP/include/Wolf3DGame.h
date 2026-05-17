#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class Wolf3DGame {
 public:
  Wolf3DGame(ILI9341Display& display, Joystick& joystick, Button& buttonA);

  void begin();
  void update();

  static constexpr int kMapW = 16;
  static constexpr int kMapH = 16;

 private:
  static constexpr int kScreenW = 288;
  static constexpr int kScreenH = 240;
  static constexpr int kHudH    = 24;
  static constexpr int kViewH   = kScreenH - kHudH;
  static constexpr int kNumCols = kScreenW;

  static constexpr float kMoveSpeed    = 2.2f;
  static constexpr float kTurnSpeedRad = 2.2f;
  static constexpr float kThreshold    = 0.45f;
  static constexpr int   kFrameMs      = 40;

  static uint16_t wallShade(float dist, bool nsWall);

  // -------------------------------------------------------------------------
  // World-sprite table (enemies + bullets). Gun is stored SEPARATELY so it
  // never clobbers enemy data on overlapping columns.
  // type: 0=none, 1=enemy, 2=bullet
  // -------------------------------------------------------------------------
  uint8_t  spriteType_[kNumCols];
  float    spriteDist_[kNumCols];
  int16_t  spriteY0_[kNumCols];
  int16_t  spriteY1_[kNumCols];
  uint8_t  spriteU_[kNumCols];    // 0–255 normalised x across sprite width

  // -------------------------------------------------------------------------
  // Gun table — entirely separate, never overwritten by world sprites
  // -------------------------------------------------------------------------
  bool     gunCol_[kNumCols];          // true if this column has gun pixels
  int16_t  gunY0_[kNumCols];
  int16_t  gunY1_[kNumCols];
  uint16_t gunColor_[kNumCols];

  // z-buffer (wall depth per column, filled by castColumn DDA)
  float zBuf_[kNumCols];

  // -------------------------------------------------------------------------
  static constexpr int kNumEnemies = 6;
  struct Enemy { float x, y; bool alive; int health; };

  static constexpr int   kNumBullets  = 4;
  static constexpr float kBulletSpeed = 8.0f;
  struct Bullet { float x, y, vx, vy; bool active; };

  // -------------------------------------------------------------------------
  float posX_, posY_, dirX_, dirY_, planeX_, planeY_;
  int   ammo_, health_, score_;
  int   prevAmmo_, prevHealth_, prevScore_;

  Enemy  enemies_[kNumEnemies];
  Bullet bullets_[kNumBullets];

  bool          gunFiring_;
  unsigned long gunFireStartMs_;
  static constexpr unsigned long kGunAnimMs  = 120;
  static constexpr unsigned long kShotCoolMs = 400;

  unsigned long lastFrameMs_;
  bool isGameOver_, prevGameOver_, prevBtnA_;
  unsigned long lastShotMs_;

  ILI9341Display& display_;
  Joystick&       joystick_;
  Button&         button_;

  // -------------------------------------------------------------------------
  void resetGame();
  void handleInput(float dtSec);
  void fireBullet();
  void updateBullets(float dtSec);
  void updateEnemies(float dtSec);
  bool mapSolid(int mx, int my) const;

  void buildSpriteTable();
  void renderColumns();
  void castColumn(int col);
  void renderHud();
  void renderGameOver();

  float projectSprite(float wx, float wy, int& x0, int& x1, int& y0, int& y1) const;
  void submitEnemy(int x0, int x1, int y0, int y1, float tY);
  void submitBullet(int x0, int x1, int y0, int y1, float tY);
  void submitGun();
  void drawDemonColumn(int col, int sy0, int sy1, uint8_t u);
  void sortEnemies(int* order, float* dist) const;
};
