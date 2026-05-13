#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class TankyGame {
 public:
  TankyGame(ILI9341Display& display, Joystick& joystick, Button& button);

  void begin();
  void update();

 private:
  // -------------------------------------------------------------------------
  // Arena layout (landscape 320×240, 10% right edge unusable → ~288 wide)
  // -------------------------------------------------------------------------
  static constexpr int kHudH   = 0;        // no dedicated HUD strip; score overlaid
  static constexpr int kArenaX = 0;
  static constexpr int kArenaY = 0;
  static constexpr int kArenaW = 288;      // 90% of 320
  static constexpr int kArenaH = 240;

  // Tank geometry
  static constexpr int kTankHalf   = 7;    // half-size of tank body square
  static constexpr int kBarrelLen  = 10;   // barrel length from centre
  static constexpr int kBarrelW    = 3;    // barrel thickness

  // Bullet geometry / physics
  static constexpr int kBulletR    = 3;    // bullet half-size
  static constexpr float kBulletSpeed  = 70.0f;   // pixels/second
  static constexpr float kTankSpeed    = 45.0f;   // pixels/second
  static constexpr float kTurnSpeedDeg = 240.0f;  // degrees/second

  // Input
  static constexpr float kThreshold = 0.45f;

  // Timings
  static constexpr unsigned long kFrameMs    = 33;  // ~30 fps
  static constexpr unsigned long kFireCoolMs = 600; // fire cooldown ms
  static constexpr unsigned long kExplodeMs  = 800; // explosion display ms

  // AI behaviour tuning
  static constexpr unsigned long kAiFireCoolMs    = 900;
  static constexpr int           kAiAimToleranceDeg = 15;
  static constexpr float         kAiPreferredDist    = 80.0f;

  // -------------------------------------------------------------------------
  struct Tank {
    float x, y;         // centre position
    int16_t angleDeg;   // heading: 0=up, 90=right, 180=down, 270=left
    float sinA, cosA;   // cached sin/cos of angleDeg, updated when angle changes
    bool alive;
    unsigned long lastFireMs;
    uint16_t color;
    uint16_t barrelColor;

    // previous-frame values for dirty-flag redraws
    float prevX, prevY;
    int16_t prevAngle;
    bool prevAlive;
  };

  struct Bullet {
    float x, y;
    float vx, vy;       // pixels/ms (float, precomputed at fire time)
    bool active;
    bool isPlayer;      // true = player's bullet, false = AI's

    float prevX, prevY;
    bool prevActive;
  };

  // -------------------------------------------------------------------------
  Tank player_;
  Tank ai_;
  Bullet playerBullet_;
  Bullet aiBullet_;

  int score_;
  int prevScore_;

  bool isGameOver_;
  bool playerWonRound_;   // true = player killed AI, false = player died
  bool needsFullRedraw_;
  bool prevGameOver_;

  unsigned long lastFrameMs_;
  unsigned long explodeStartMs_;

  // Input edge tracking
  bool prevBtnA_;

  ILI9341Display& display_;
  Joystick& joystick_;
  Button& button_;

  // -------------------------------------------------------------------------
  void resetRound(bool fullReset);
  void updatePlayer(float dtMs);
  void updateAi(float dtMs);
  void updateBullet(Bullet& b, float dtMs);
  bool checkCollision(const Bullet& b, const Tank& t) const;
  bool tanksColliding() const;
  void fireBullet(Bullet& b, const Tank& from);
  void setAngle(Tank& t, int16_t deg);  // sets angleDeg and refreshes sinA/cosA
  int16_t normalizeAngle(int16_t a) const;
  int16_t angleDiff(int16_t from, int16_t to) const;  // signed [-180,180]

  // Rendering
  void render(float dtMs);
  void fullRedraw();
  void drawArenaEdge();
  void drawHud();      // score-only refresh
  void drawHudFull();  // full HUD including static labels
  void eraseTank(const Tank& t);
  void drawTank(const Tank& t);
  void eraseBullet(const Bullet& b);
  void drawBullet(const Bullet& b);
  void drawExplosion(float x, float y, uint16_t color);
  void drawGameOverScreen();
};
