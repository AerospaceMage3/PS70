#include "TankyGame.h"
#include <math.h>

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
static float sinDeg(int16_t d) { return sinf(d * (float)M_PI / 180.0f); }
static float cosDeg(int16_t d) { return cosf(d * (float)M_PI / 180.0f); }

// ---------------------------------------------------------------------------
TankyGame::TankyGame(ILI9341Display& display, Joystick& joystick, Button& button)
    : display_(display), joystick_(joystick), button_(button),
      score_(0), prevScore_(-1),
      isGameOver_(false), playerWonRound_(false),
      needsFullRedraw_(true), prevGameOver_(false),
      lastFrameMs_(0), explodeStartMs_(0),
      prevBtnA_(false) {
  // zero-init tanks / bullets
  memset(&player_, 0, sizeof(player_));
  memset(&ai_,     0, sizeof(ai_));
  memset(&playerBullet_, 0, sizeof(playerBullet_));
  memset(&aiBullet_,     0, sizeof(aiBullet_));
}

void TankyGame::begin() {
  randomSeed(analogRead(A0));
  score_ = 0;
  prevScore_ = -1;
  resetRound(true);
}

// Place the two tanks at opposite ends, facing each other.
void TankyGame::resetRound(bool fullReset) {
  if (fullReset) {
    score_ = 0;
    prevScore_ = -1;
  }

  player_.x        = kArenaX + kArenaW * 0.25f;
  player_.y        = kArenaY + kArenaH * 0.5f;
  player_.angleDeg = -1;  // force setAngle to recompute
  player_.sinA = 0; player_.cosA = 0;
  setAngle(player_, 90);   // facing right (toward AI)
  player_.alive    = true;
  player_.lastFireMs = 0;
  player_.color       = ILI9341_GREEN;
  player_.barrelColor = 0x07E0;
  player_.prevX = player_.x + 1;  // force redraw
  player_.prevAngle = -1;

  ai_.x        = kArenaX + kArenaW * 0.75f;
  ai_.y        = kArenaY + kArenaH * 0.5f;
  ai_.angleDeg = -1;
  ai_.sinA = 0; ai_.cosA = 0;
  setAngle(ai_, 270);  // facing left (toward player)
  ai_.alive    = true;
  ai_.lastFireMs = 0;
  ai_.color       = ILI9341_RED;
  ai_.barrelColor = 0xF800;
  ai_.prevX = ai_.x + 1;
  ai_.prevAngle = -1;

  playerBullet_.active = false;
  playerBullet_.prevActive = false;
  aiBullet_.active = false;
  aiBullet_.prevActive = false;

  isGameOver_      = false;
  prevGameOver_    = false;
  needsFullRedraw_ = true;
  lastFrameMs_     = millis();
  explodeStartMs_  = 0;
  prevBtnA_        = false;
}

// ---------------------------------------------------------------------------
// Main update
// ---------------------------------------------------------------------------
void TankyGame::update() {
  unsigned long now = millis();
  float dtMs = (float)(now - lastFrameMs_);
  if (dtMs < kFrameMs) {
    delay(2);
    return;
  }
  lastFrameMs_ = now;

  bool btnA = button_.isPressed();
  bool btnAEdge = btnA && !prevBtnA_;
  prevBtnA_ = btnA;

  // --- Game-over / explosion state ---
  if (isGameOver_) {
    if (!prevGameOver_) {
      drawExplosion(playerWonRound_ ? ai_.x : player_.x,
                    playerWonRound_ ? ai_.y : player_.y,
                    playerWonRound_ ? ILI9341_RED : ILI9341_GREEN);
      drawGameOverScreen();
      prevGameOver_ = true;
    }
    if (btnAEdge) {
      if (playerWonRound_) {
        // Won a round: start next battle, keep score
        resetRound(false);
        fullRedraw();
      } else {
        // Lost: full reset
        resetRound(true);
        fullRedraw();
      }
    }
    delay(20);
    return;
  }

  updatePlayer(dtMs);
  if (btnAEdge) {
    unsigned long t = millis();
    if (!playerBullet_.active && (t - player_.lastFireMs >= kFireCoolMs)) {
      fireBullet(playerBullet_, player_);
      player_.lastFireMs = t;
    }
  }

  updateAi(dtMs);
  updateBullet(playerBullet_, dtMs);
  updateBullet(aiBullet_, dtMs);

  // --- Collision checks ---
  if (playerBullet_.active && checkCollision(playerBullet_, ai_)) {
    playerBullet_.active = false;
    score_++;
    playerWonRound_ = true;
    isGameOver_ = true;
    ai_.alive = false;
  }
  if (aiBullet_.active && checkCollision(aiBullet_, player_)) {
    aiBullet_.active = false;
    playerWonRound_ = false;
    isGameOver_ = true;
    player_.alive = false;
  }
  if (tanksColliding()) {
    playerWonRound_ = false;
    isGameOver_ = true;
    player_.alive = false;
  }

  render(dtMs);
}

// ---------------------------------------------------------------------------
// Player input
// ---------------------------------------------------------------------------
void TankyGame::updatePlayer(float dtMs) {
  Joystick::NormalizedReading joy = joystick_.readNormalized();

  // joy.y left/right → rotate (per CLAUDE.md convention: y>=thresh=left)
  // We flip for tank: joy.y >= thresh = rotate CCW (left), joy.y <= -thresh = CW
  float turnInput = 0.0f;
  if (joy.y <= -kThreshold) turnInput =  1.0f;   // rotate CW (right)
  if (joy.y >=  kThreshold) turnInput = -1.0f;   // rotate CCW (left)

  // joy.x up/down → drive (x >= thresh = "down" in CLAUDE.md = drive forward here)
  float driveInput = 0.0f;
  if (joy.x <= -kThreshold) driveInput =  1.0f;  // forward
  if (joy.x >=  kThreshold) driveInput = -1.0f;  // backward

  // Apply rotation
  float dAngle = turnInput * kTurnSpeedDeg * (dtMs / 1000.0f);
  setAngle(player_, player_.angleDeg + (int16_t)dAngle);

  // Apply drive
  if (fabsf(driveInput) > 0.01f) {
    float speed = driveInput * kTankSpeed * (dtMs / 1000.0f);
    float nx = player_.x + player_.sinA * speed;
    float ny = player_.y - player_.cosA * speed;
    // Clamp to arena
    nx = max((float)(kArenaX + kTankHalf + 1), min((float)(kArenaX + kArenaW - kTankHalf - 1), nx));
    ny = max((float)(kArenaY + kTankHalf + 1), min((float)(kArenaY + kArenaH - kTankHalf - 1), ny));
    player_.x = nx;
    player_.y = ny;
  }
}

// ---------------------------------------------------------------------------
// AI logic
// ---------------------------------------------------------------------------
void TankyGame::updateAi(float dtMs) {
  float dx = player_.x - ai_.x;
  float dy = player_.y - ai_.y;
  float dist = sqrtf(dx * dx + dy * dy);

  // Angle from AI to player (0=up, 90=right, ...)
  int16_t targetAngle = (int16_t)(atan2f(dx, -dy) * 180.0f / (float)M_PI);
  targetAngle = normalizeAngle(targetAngle);

  int16_t diff = angleDiff(ai_.angleDeg, targetAngle);

  // Rotate toward player
  float maxTurn = kTurnSpeedDeg * (dtMs / 1000.0f);
  if (abs(diff) > 2) {
    float turn = (diff > 0) ? min((float)diff, maxTurn)
                            : max((float)diff, -maxTurn);
    setAngle(ai_, ai_.angleDeg + (int16_t)turn);
  }

  // Drive: close gap if too far, back off if too close
  float driveInput = 0.0f;
  if (dist > kAiPreferredDist + 20) driveInput =  1.0f;
  if (dist < kAiPreferredDist - 20) driveInput = -1.0f;

  // Small random juke every ~1.5 s to avoid being predictable
  static unsigned long lastJukeMs = 0;
  static float jukeDir = 0.0f;
  unsigned long now = millis();
  if (now - lastJukeMs > 1500) {
    lastJukeMs = now;
    jukeDir = (random(0, 2) == 0) ? 20.0f : -20.0f;
  }
  // Apply a small sideways juke offset to angle when driving
  int16_t driveAngle = normalizeAngle(ai_.angleDeg + (int16_t)jukeDir);

  if (fabsf(driveInput) > 0.01f) {
    float speed = driveInput * kTankSpeed * (dtMs / 1000.0f);
    float nx = ai_.x + sinDeg(driveAngle) * speed;
    float ny = ai_.y - cosDeg(driveAngle) * speed;
    nx = max((float)(kArenaX + kTankHalf + 1), min((float)(kArenaX + kArenaW - kTankHalf - 1), nx));
    ny = max((float)(kArenaY + kTankHalf + 1), min((float)(kArenaY + kArenaH - kTankHalf - 1), ny));
    ai_.x = nx;
    ai_.y = ny;
  }

  // Fire when aimed closely enough
  if (!aiBullet_.active && abs(diff) <= kAiAimToleranceDeg &&
      (now - ai_.lastFireMs >= kAiFireCoolMs)) {
    fireBullet(aiBullet_, ai_);
    ai_.lastFireMs = now;
  }
}

// ---------------------------------------------------------------------------
// Bullet update
// ---------------------------------------------------------------------------
void TankyGame::updateBullet(Bullet& b, float dtMs) {
  if (!b.active) return;
  b.x += b.vx * dtMs;
  b.y += b.vy * dtMs;
  // Deactivate on arena wall
  if (b.x < kArenaX || b.x > kArenaX + kArenaW ||
      b.y < kArenaY || b.y > kArenaY + kArenaH) {
    b.active = false;
  }
}

bool TankyGame::checkCollision(const Bullet& b, const Tank& t) const {
  if (!b.active || !t.alive) return false;
  float dx = b.x - t.x;
  float dy = b.y - t.y;
  float hitR = kTankHalf + kBulletR;
  return (dx * dx + dy * dy) < (hitR * hitR);
}

bool TankyGame::tanksColliding() const {
  float dx = player_.x - ai_.x;
  float dy = player_.y - ai_.y;
  float minDist = kTankHalf * 2 + 2;
  return (dx * dx + dy * dy) < (minDist * minDist);
}

void TankyGame::fireBullet(Bullet& b, const Tank& from) {
  b.active   = true;
  b.isPlayer = (&from == &player_);
  b.x = from.x + from.sinA * (kTankHalf + kBarrelLen + kBulletR + 1);
  b.y = from.y - from.cosA * (kTankHalf + kBarrelLen + kBulletR + 1);
  float speedPerMs = kBulletSpeed / 1000.0f;
  b.vx =  from.sinA * speedPerMs;
  b.vy = -from.cosA * speedPerMs;
  b.prevActive = false;
}

// ---------------------------------------------------------------------------
// Angle helpers
// ---------------------------------------------------------------------------
void TankyGame::setAngle(Tank& t, int16_t deg) {
  deg = normalizeAngle(deg);
  if (deg == t.angleDeg) return;
  t.angleDeg = deg;
  t.sinA = sinDeg(deg);
  t.cosA = cosDeg(deg);
}

int16_t TankyGame::normalizeAngle(int16_t a) const {
  a %= 360;
  if (a < 0) a += 360;
  return a;
}

int16_t TankyGame::angleDiff(int16_t from, int16_t to) const {
  int16_t d = normalizeAngle(to - from);
  if (d > 180) d -= 360;
  return d;
}

// ---------------------------------------------------------------------------
// Rendering
// ---------------------------------------------------------------------------
void TankyGame::render(float /*dtMs*/) {
  if (needsFullRedraw_) {
    fullRedraw();
    needsFullRedraw_ = false;
    return;
  }

  // Erase old bullet positions
  eraseBullet(playerBullet_);
  eraseBullet(aiBullet_);

  // Erase old tank positions if moved or rotated
  bool playerMoved = (fabsf(player_.x - player_.prevX) > 0.5f ||
                      fabsf(player_.y - player_.prevY) > 0.5f ||
                      player_.angleDeg != player_.prevAngle);
  bool aiMoved     = (fabsf(ai_.x - ai_.prevX) > 0.5f ||
                      fabsf(ai_.y - ai_.prevY) > 0.5f ||
                      ai_.angleDeg != ai_.prevAngle);

  if (playerMoved) eraseTank(player_);
  if (aiMoved)     eraseTank(ai_);

  // Draw bullets at new positions
  drawBullet(playerBullet_);
  drawBullet(aiBullet_);

  // Draw tanks at new positions
  if (playerMoved) drawTank(player_);
  if (aiMoved)     drawTank(ai_);

  // Update snapshots
  if (playerMoved) {
    player_.prevX     = player_.x;
    player_.prevY     = player_.y;
    player_.prevAngle = player_.angleDeg;
  }
  if (aiMoved) {
    ai_.prevX     = ai_.x;
    ai_.prevY     = ai_.y;
    ai_.prevAngle = ai_.angleDeg;
  }
  playerBullet_.prevX = playerBullet_.x;
  playerBullet_.prevY = playerBullet_.y;
  playerBullet_.prevActive = playerBullet_.active;
  aiBullet_.prevX = aiBullet_.x;
  aiBullet_.prevY = aiBullet_.y;
  aiBullet_.prevActive = aiBullet_.active;

  // HUD score
  if (score_ != prevScore_) {
    drawHud();
    prevScore_ = score_;
  }
}

void TankyGame::fullRedraw() {
  display_.clear(ILI9341_BLACK);
  drawArenaEdge();
  drawHudFull();
  drawTank(player_);
  drawTank(ai_);
  player_.prevX = player_.x; player_.prevY = player_.y; player_.prevAngle = player_.angleDeg;
  ai_.prevX = ai_.x; ai_.prevY = ai_.y; ai_.prevAngle = ai_.angleDeg;
  prevScore_ = score_;
}

void TankyGame::drawArenaEdge() {
  display_.drawRect(kArenaX, kArenaY, kArenaW, kArenaH, ILI9341_WHITE);
}

void TankyGame::drawHud() {
  // Score overlaid in top-left corner; clear just that region
  display_.fillRect(2, 2, 70, 14, ILI9341_BLACK);
  display_.drawText(2, 2, "S:" + String(score_), ILI9341_YELLOW, 1, ILI9341_BLACK);
}

void TankyGame::drawHudFull() {
  // Same as drawHud — no separate strip to paint
  drawHud();
}

// ---------------------------------------------------------------------------
// Tank drawing — body is a filled square, barrel is a thin rectangle
// projected from centre in facing direction.
// ---------------------------------------------------------------------------
void TankyGame::eraseTank(const Tank& t) {
  int cx = (int)t.prevX;
  int cy = (int)t.prevY;
  // Erase bounding box that covers body + barrel tip (no border repair needed
  // — tanks are clamped at least kTankHalf+1 px inside the arena wall)
  int pad = kTankHalf + kBarrelLen + 2;
  display_.fillRect(cx - pad, cy - pad, pad * 2, pad * 2, ILI9341_BLACK);
}

void TankyGame::drawTank(const Tank& t) {
  if (!t.alive) return;
  int cx = (int)t.x;
  int cy = (int)t.y;

  // Tank body
  display_.fillRect(cx - kTankHalf, cy - kTankHalf,
                    kTankHalf * 2, kTankHalf * 2, t.color);

  // Barrel tip and base midpoints using cached sin/cos
  float tipX  = cx + t.sinA * (kTankHalf + kBarrelLen);
  float tipY  = cy - t.cosA * (kTankHalf + kBarrelLen);
  float baseX = cx + t.sinA * kTankHalf;
  float baseY = cy - t.cosA * kTankHalf;

  // Draw barrel as a fat cross: a narrow rect along the axis
  // We approximate by drawing two overlapping rects (horizontal & vertical
  // projections), which looks good at small sizes without needing drawLine.
  int hw = kBarrelW / 2;

  // Wider direction of barrel rect depends on angle
  // Use the rect bounding the barrel segment
  int x0 = (int)min(baseX, tipX) - hw;
  int y0 = (int)min(baseY, tipY) - hw;
  int x1 = (int)max(baseX, tipX) + hw;
  int y1 = (int)max(baseY, tipY) + hw;
  display_.fillRect(x0, y0, x1 - x0 + 1, y1 - y0 + 1, t.barrelColor);
}

void TankyGame::eraseBullet(const Bullet& b) {
  if (!b.prevActive) return;
  int bx = (int)b.prevX;
  int by = (int)b.prevY;
  display_.fillRect(bx - kBulletR - 1, by - kBulletR - 1,
                    kBulletR * 2 + 2, kBulletR * 2 + 2, ILI9341_BLACK);
}

void TankyGame::drawBullet(const Bullet& b) {
  if (!b.active) return;
  uint16_t color = b.isPlayer ? ILI9341_YELLOW : 0xF81F;  // yellow / magenta
  display_.fillRect((int)b.x - kBulletR, (int)b.y - kBulletR,
                    kBulletR * 2, kBulletR * 2, color);
}

void TankyGame::drawExplosion(float x, float y, uint16_t color) {
  int ix = (int)x, iy = (int)y;
  // Concentric filled squares expanding outward (cheap explosion)
  for (int r = 4; r <= 20; r += 4) {
    display_.fillRect(ix - r, iy - r, r * 2, r * 2, color);
    delay(40);
    display_.fillRect(ix - r, iy - r, r * 2, r * 2, ILI9341_BLACK);
  }
  // Leave a small debris mark
  display_.fillRect(ix - 6, iy - 6, 12, 12, 0x4208);  // dark grey
}

void TankyGame::drawGameOverScreen() {
  if (playerWonRound_) {
    display_.drawText(60, kArenaY + kArenaH / 2 - 20,
                      "ENEMY DOWN!", ILI9341_YELLOW, 2, ILI9341_BLACK);
    display_.drawText(60, kArenaY + kArenaH / 2 + 4,
                      "Score: " + String(score_), ILI9341_WHITE, 2, ILI9341_BLACK);
    display_.drawText(30, kArenaY + kArenaH / 2 + 28,
                      "A: Next Round", ILI9341_GREEN, 1, ILI9341_BLACK);
  } else {
    display_.drawText(60, kArenaY + kArenaH / 2 - 20,
                      "YOU DIED!", ILI9341_RED, 2, ILI9341_BLACK);
    display_.drawText(60, kArenaY + kArenaH / 2 + 4,
                      "Score: " + String(score_), ILI9341_WHITE, 2, ILI9341_BLACK);
    display_.drawText(30, kArenaY + kArenaH / 2 + 28,
                      "A: Restart", ILI9341_YELLOW, 1, ILI9341_BLACK);
  }
}
