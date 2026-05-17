#include "Wolf3DGame.h"
#include <math.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Map
// ---------------------------------------------------------------------------
static const uint8_t kMap[Wolf3DGame::kMapH][Wolf3DGame::kMapW] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
  {1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

static inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

// ---------------------------------------------------------------------------
// Demon sprite silhouette — 16 columns × 16 rows, 1 = opaque, 0 = transparent.
// Coordinates: col 0=left edge, row 0=top of sprite.
// Divided into 3 vertical zones: head (rows 0-4), torso (rows 5-11), legs (rows 12-15).
// Head: roughly circular. Torso: wider with arm stumps. Legs: two pillars.
// ---------------------------------------------------------------------------
// clang-format off
static const uint8_t kDemonSil[16][16] = {
  // cols: 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15   row
  /*  0 */ {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // head top
  /*  1 */ {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  /*  2 */ {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  /*  3 */ {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  /*  4 */ {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},  // head bottom
  /*  5 */ {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // neck
  /*  6 */ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},  // shoulder
  /*  7 */ {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},  // upper arm gap
  /*  8 */ {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1},
  /*  9 */ {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},  // torso
  /* 10 */ {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  /* 11 */ {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // waist
  /* 12 */ {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},  // upper leg gap
  /* 13 */ {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
  /* 14 */ {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},  // lower legs
  /* 15 */ {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
};
// clang-format on

// Per-row demon colour (indexed 0–15 → maps to sprite row)
static uint16_t demonRowColor(int row) {
  if (row <= 4) return rgb565(150, 30, 30);   // head: dark red
  if (row == 5) return rgb565(100, 20, 20);   // neck: darker
  if (row <= 8) return rgb565(80, 130, 50);   // shoulders/arms: olive
  if (row <= 11) return rgb565(60, 100, 40);  // torso: dark olive
  return rgb565(55, 38, 18);                  // legs: dark brown
}

// Eye highlights drawn on top at specific silhouette positions
static inline bool isDemonEye(int row, int col16) {
  // Left eye: rows 2-3, cols 4-5. Right eye: rows 2-3, cols 10-11.
  return (row >= 2 && row <= 3) &&
         ((col16 >= 4 && col16 <= 5) || (col16 >= 10 && col16 <= 11));
}

static inline bool isDemonPupil(int row, int col16) {
  return (row == 2) && (col16 == 4 || col16 == 10);
}

// Wall shading palette
uint16_t Wolf3DGame::wallShade(float dist, bool nsWall) {
  if (dist < 0.5f)  dist = 0.5f;
  if (dist > 12.0f) dist = 12.0f;
  int band = 7 - (int)(dist / 12.0f * 8.0f);
  if (band < 0) band = 0;
  if (nsWall && band > 0) band--;
  static const uint16_t pal[8] = {
    rgb565(32,36,40),   rgb565(55,62,68),
    rgb565(80,90,100),  rgb565(105,118,130),
    rgb565(130,145,160),rgb565(160,175,190),
    rgb565(190,205,215),rgb565(220,228,235),
  };
  return pal[band];
}

// ---------------------------------------------------------------------------
Wolf3DGame::Wolf3DGame(ILI9341Display& display, Joystick& joystick, Button& buttonA)
    : display_(display), joystick_(joystick), button_(buttonA),
      posX_(0), posY_(0), dirX_(0), dirY_(0), planeX_(0), planeY_(0),
      ammo_(30), health_(100), score_(0),
      prevAmmo_(-1), prevHealth_(-1), prevScore_(-1),
      gunFiring_(false), gunFireStartMs_(0),
      lastFrameMs_(0), isGameOver_(false), prevGameOver_(false),
      prevBtnA_(false), lastShotMs_(0) {
  memset(enemies_, 0, sizeof(enemies_));
  memset(bullets_, 0, sizeof(bullets_));
  memset(zBuf_,    0, sizeof(zBuf_));
  memset(spriteType_, 0, sizeof(spriteType_));
  memset(spriteDist_, 0, sizeof(spriteDist_));
  memset(gunCol_,     0, sizeof(gunCol_));
}

void Wolf3DGame::begin() { resetGame(); }

void Wolf3DGame::resetGame() {
  posX_ = 1.5f; posY_ = 1.5f;
  dirX_ = 1.0f; dirY_ = 0.0f;
  planeX_ = 0.0f; planeY_ = 0.66f;

  ammo_ = 40; health_ = 100; score_ = 0;
  prevAmmo_ = prevHealth_ = prevScore_ = -1;
  gunFiring_ = false; isGameOver_ = false; prevGameOver_ = false;
  prevBtnA_ = false; lastShotMs_ = 0; lastFrameMs_ = millis();

  randomSeed(micros());
  for (int i = 0; i < kNumEnemies; i++) {
    float ex = 3.5f, ey = 5.5f;  // fallback if all attempts fail
    for (int attempt = 0; attempt < 200; attempt++) {
      int mx = (int)random(1, kMapW - 1);
      int my = (int)random(1, kMapH - 1);
      if (mapSolid(mx, my)) continue;
      float cx = mx + 0.5f, cy = my + 0.5f;
      float pdx = cx - posX_, pdy = cy - posY_;
      if (pdx*pdx + pdy*pdy < 9.0f) continue;
      bool tooClose = false;
      for (int j = 0; j < i; j++) {
        float edx = cx - enemies_[j].x, edy = cy - enemies_[j].y;
        if (edx*edx + edy*edy < 2.25f) { tooClose = true; break; }
      }
      if (tooClose) continue;
      ex = cx; ey = cy;
      break;
    }
    enemies_[i] = {ex, ey, true, 1};
  }
  for (int i = 0; i < kNumBullets; i++)
    bullets_[i].active = false;

  display_.clear(ILI9341_BLACK);
  buildSpriteTable();
  renderColumns();
  renderHud();
}

bool Wolf3DGame::mapSolid(int mx, int my) const {
  if (mx < 0 || mx >= kMapW || my < 0 || my >= kMapH) return true;
  return kMap[my][mx] != 0;
}

// ---------------------------------------------------------------------------
void Wolf3DGame::update() {
  unsigned long now = millis();
  float dtMs = (float)(now - lastFrameMs_);
  if (dtMs < kFrameMs) { delay(2); return; }
  lastFrameMs_ = now;
  float dtSec = dtMs / 1000.0f;

  bool btnA    = button_.isPressed();
  bool btnEdge = btnA && !prevBtnA_;
  prevBtnA_    = btnA;

  if (isGameOver_) {
    if (!prevGameOver_) { renderGameOver(); prevGameOver_ = true; }
    if (btnEdge) resetGame();
    delay(20);
    return;
  }

  handleInput(dtSec);

  if (btnEdge && ammo_ > 0) {
    unsigned long t = millis();
    if (t - lastShotMs_ >= kShotCoolMs) {
      fireBullet();
      lastShotMs_ = t;
      gunFiring_ = true;
      gunFireStartMs_ = t;
    }
  }
  if (gunFiring_ && (millis() - gunFireStartMs_ >= kGunAnimMs))
    gunFiring_ = false;

  updateBullets(dtSec);
  updateEnemies(dtSec);

  if (health_ <= 0) { isGameOver_ = true; return; }
  bool allDead = true;
  for (int i = 0; i < kNumEnemies; i++) if (enemies_[i].alive) { allDead = false; break; }
  if (allDead) { isGameOver_ = true; return; }

  buildSpriteTable();
  renderColumns();
  renderHud();
}

// ---------------------------------------------------------------------------
void Wolf3DGame::handleInput(float dtSec) {
  Joystick::NormalizedReading joy = joystick_.readNormalized();
  float moveSpeed = kMoveSpeed * dtSec;
  float rotSpeed  = kTurnSpeedRad * dtSec;

  float fwd = 0.0f;
  if (joy.x <= -kThreshold) fwd =  1.0f;
  if (joy.x >=  kThreshold) fwd = -1.0f;
  if (fwd != 0.0f) {
    float nx = posX_ + dirX_ * moveSpeed * fwd;
    float ny = posY_ + dirY_ * moveSpeed * fwd;
    if (!mapSolid((int)nx,    (int)posY_)) posX_ = nx;
    if (!mapSolid((int)posX_, (int)ny))    posY_ = ny;
  }

  float rot = 0.0f;
  if (joy.y >=  kThreshold) rot = -1.0f;
  if (joy.y <= -kThreshold) rot =  1.0f;
  if (rot != 0.0f) {
    float a = rot * rotSpeed, c = cosf(a), s = sinf(a);
    float odx = dirX_, opx = planeX_;
    dirX_   =  odx * c - dirY_   * s;
    dirY_   =  odx * s + dirY_   * c;
    planeX_ =  opx * c - planeY_ * s;
    planeY_ =  opx * s + planeY_ * c;
  }
}

// ---------------------------------------------------------------------------
void Wolf3DGame::fireBullet() {
  ammo_--;
  for (int i = 0; i < kNumBullets; i++) {
    if (!bullets_[i].active) {
      // Spawn slightly right of centre to match barrel position (60% of screen
      // width = camera X offset of ~+0.2 from centre). Offset in the right
      // perpendicular direction (planeX/Y is the camera plane vector).
      const float kBarrelOffset = 0.18f;  // rightward offset in camera-plane units
      bullets_[i] = {
        posX_ + dirX_*0.3f + planeX_*kBarrelOffset,
        posY_ + dirY_*0.3f + planeY_*kBarrelOffset,
        dirX_*kBulletSpeed,
        dirY_*kBulletSpeed,
        true
      };
      break;
    }
  }
}

void Wolf3DGame::updateBullets(float dtSec) {
  for (int i = 0; i < kNumBullets; i++) {
    if (!bullets_[i].active) continue;
    bullets_[i].x += bullets_[i].vx * dtSec;
    bullets_[i].y += bullets_[i].vy * dtSec;
    if (mapSolid((int)bullets_[i].x, (int)bullets_[i].y)) {
      bullets_[i].active = false; continue;
    }
    for (int e = 0; e < kNumEnemies; e++) {
      if (!enemies_[e].alive) continue;
      float dx = bullets_[i].x - enemies_[e].x;
      float dy = bullets_[i].y - enemies_[e].y;
      if (dx*dx + dy*dy < 0.3f*0.3f) {
        enemies_[e].alive = false;
        bullets_[i].active = false;
        score_++;
        break;
      }
    }
  }
}

void Wolf3DGame::updateEnemies(float dtSec) {
  for (int i = 0; i < kNumEnemies; i++) {
    if (!enemies_[i].alive) continue;
    float dx = posX_ - enemies_[i].x, dy = posY_ - enemies_[i].y;
    float dist = sqrtf(dx*dx + dy*dy);
    if (dist < 0.01f) continue;
    float speed = kMoveSpeed * 0.45f * dtSec;
    float nx = enemies_[i].x + (dx/dist)*speed;
    float ny = enemies_[i].y + (dy/dist)*speed;
    if (!mapSolid((int)nx,            (int)enemies_[i].y)) enemies_[i].x = nx;
    if (!mapSolid((int)enemies_[i].x, (int)ny))            enemies_[i].y = ny;
    if (dist < 0.75f) { health_ -= 25; if (health_ < 0) health_ = 0; }
  }
}

// ---------------------------------------------------------------------------
// Sprite projection
// ---------------------------------------------------------------------------
float Wolf3DGame::projectSprite(float wx, float wy,
                                int& x0, int& x1, int& y0, int& y1) const {
  float sx = wx - posX_, sy = wy - posY_;
  float invDet = 1.0f / (planeX_*dirY_ - dirX_*planeY_);
  float tX = invDet * ( dirY_*sx - dirX_*sy);
  float tY = invDet * (-planeY_*sx + planeX_*sy);
  if (tY <= 0.05f) return 0.0f;
  int cx = (int)((kNumCols/2) * (1.0f + tX/tY));
  int h  = abs((int)(kViewH / tY));
  x0 = cx - h/2; x1 = cx + h/2;
  y0 = kViewH/2 - h/2; y1 = kViewH/2 + h/2;
  if (y0 < 0)       y0 = 0;
  if (y1 >= kViewH) y1 = kViewH - 1;
  return tY;
}

// ---------------------------------------------------------------------------
// Submit helpers
// ---------------------------------------------------------------------------

// Enemies → world sprite table (nearer overwrites farther)
void Wolf3DGame::submitEnemy(int x0, int x1, int y0, int y1, float tY) {
  int sprW = (x1 - x0) + 1; if (sprW <= 0) return;
  for (int x = x0; x <= x1; x++) {
    if (x < 0 || x >= kNumCols) continue;
    if (spriteType_[x] != 0 && tY >= spriteDist_[x]) continue;
    spriteType_[x] = 1;
    spriteDist_[x] = tY;
    spriteY0_[x]   = (int16_t)y0;
    spriteY1_[x]   = (int16_t)y1;
    spriteU_[x]    = (uint8_t)(255 * (x - x0) / sprW);
  }
}

// Bullets → gun table (drawn over gun if closer, separate from enemies)
void Wolf3DGame::submitBullet(int x0, int x1, int y0, int y1, float tY) {
  for (int x = x0; x <= x1; x++) {
    if (x < 0 || x >= kNumCols) continue;
    // Only overwrite gun pixels if bullet is closer than current gun entry
    if (gunCol_[x] && tY >= spriteDist_[x]) continue;
    gunCol_[x]    = true;
    spriteDist_[x]= tY;          // reuse dist slot for bullet z (gun uses 0.001)
    gunY0_[x]     = (int16_t)y0;
    gunY1_[x]     = (int16_t)y1;
    gunColor_[x]  = rgb565(255, 200, 40);
  }
}

// Gun → gun table (depth 0.001 — always behind bullets which are in the world)
void Wolf3DGame::submitGun() {
  int recoilPx = gunFiring_ ? 0 : 10;

  const int kGunCX      = kScreenW * 60 / 100;
  const int kGunBaseY   = kViewH - 1 + recoilPx;
  const int kGunMaxH    = kViewH * 52 / 100;
  const int kBoreHalf   = 5;
  const int kBarrelHalf = 16;
  const int kSpreadL    = kGunCX - kScreenW * 28 / 100;
  const int kSpreadR    = kGunCX + kScreenW * 32 / 100;

  uint16_t bore      = gunFiring_ ? rgb565(255,210,50) : rgb565(18,18,22);
  uint16_t barrelL   = rgb565(155,160,170);
  uint16_t barrelR   = rgb565(55,55,65);
  uint16_t barrelMid = rgb565(95,95,108);
  uint16_t receiver  = rgb565(80,82,92);
  uint16_t wood      = rgb565(130,70,28);
  uint16_t woodDark  = rgb565(78,42,14);

  for (int col = kSpreadL; col <= kSpreadR; col++) {
    if (col < 0 || col >= kNumCols) continue;
    int dx = col - kGunCX, absDx = abs(dx);

    int gunH;
    if (absDx <= kBarrelHalf)  gunH = kGunMaxH - absDx * 3;
    else                       gunH = kGunMaxH - kBarrelHalf*3 - (absDx-kBarrelHalf)*6;
    if (gunH <= 1) continue;

    int y0 = kGunBaseY - gunH + 1;
    if (y0 < 0) y0 = 0;
    int y1 = min(kGunBaseY, kViewH - 1);

    uint16_t c;
    if (absDx <= kBoreHalf)
      c = bore;
    else if (absDx <= kBarrelHalf)
      c = (dx < 0) ? barrelL : (absDx < kBarrelHalf-2 ? barrelMid : barrelR);
    else {
      int sdx = absDx - kBarrelHalf;
      c = (dx > 0) ? ((sdx<8)?receiver:(sdx<22?wood:woodDark))
                   : ((sdx<4)?receiver:barrelR);
    }

    // Only write gun if a bullet hasn't already claimed this column
    if (!gunCol_[col]) {
      gunCol_[col]   = true;
      gunY0_[col]    = (int16_t)y0;
      gunY1_[col]    = (int16_t)y1;
      gunColor_[col] = c;
    }
  }
}

// ---------------------------------------------------------------------------
// Build table — enemies (far→near), bullets, gun (always on top)
// ---------------------------------------------------------------------------
void Wolf3DGame::buildSpriteTable() {
  // Reset world-sprite table
  memset(spriteType_, 0, sizeof(spriteType_));
  memset(spriteDist_, 0, sizeof(spriteDist_));
  // Reset gun table
  memset(gunCol_, 0, sizeof(gunCol_));

  // Enemies into world-sprite table (far→near so nearer overwrites)
  int order[kNumEnemies]; float dist[kNumEnemies];
  sortEnemies(order, dist);
  for (int si = 0; si < kNumEnemies; si++) {
    int i = order[si];
    if (!enemies_[i].alive) continue;
    int x0,x1,y0,y1;
    float tY = projectSprite(enemies_[i].x, enemies_[i].y, x0,x1,y0,y1);
    if (tY <= 0.0f) continue;
    submitEnemy(x0,x1,y0,y1,tY);
  }

  // Bullets into gun table FIRST (so submitGun won't overwrite them)
  for (int i = 0; i < kNumBullets; i++) {
    if (!bullets_[i].active) continue;
    int x0,x1,y0,y1;
    float tY = projectSprite(bullets_[i].x, bullets_[i].y, x0,x1,y0,y1);
    if (tY <= 0.0f) continue;
    int cx = (x0+x1)/2, cy = kViewH/2;
    x0=cx-3; x1=cx+3; y0=cy-3; y1=cy+3;
    if (y0<0) y0=0; if (y1>=kViewH) y1=kViewH-1;
    submitBullet(x0,x1,y0,y1,tY);
  }

  // Gun into gun table last — skips columns already claimed by bullets
  submitGun();
}

// ---------------------------------------------------------------------------
// Render pass — single sweep, no separate sprite/gun passes
// ---------------------------------------------------------------------------
void Wolf3DGame::renderColumns() {
  for (int col = 0; col < kNumCols; col++) castColumn(col);
}

// ---------------------------------------------------------------------------
// Draw one demon sprite column given normalised u (0–255 across sprite width)
// and the clamped screen y range [sy0, sy1].
// Uses the kDemonSil silhouette to skip transparent rows, drawing run-length
// segments of same-colour opaque pixels with drawVLine for minimal SPI calls.
// ---------------------------------------------------------------------------
void Wolf3DGame::drawDemonColumn(int col, int sy0, int sy1, uint8_t u) {
  int sprH = sy1 - sy0 + 1;
  if (sprH <= 0) return;

  // Map u (0–255) to silhouette column 0–15
  int sCol = (int)u * 16 / 256;
  if (sCol > 15) sCol = 15;

  // Walk rows top to bottom, batch consecutive opaque rows of the same colour
  uint16_t runColor = 0;
  int      runStart = -1;

  for (int sy = sy0; sy <= sy1; sy++) {
    // Map screen y to silhouette row 0–15
    int sRow = (sy - sy0) * 16 / sprH;
    if (sRow > 15) sRow = 15;

    if (!kDemonSil[sRow][sCol]) {
      // Transparent — flush any run
      if (runStart >= 0) {
        display_.drawVLine(col, runStart, sy - runStart, runColor);
        runStart = -1;
      }
      continue;
    }

    // Pick colour for this row
    uint16_t c;
    if (isDemonPupil(sRow, sCol))      c = rgb565(0,0,0);
    else if (isDemonEye(sRow, sCol))   c = rgb565(255,220,0);
    else                               c = demonRowColor(sRow);

    if (runStart < 0) {
      runStart = sy; runColor = c;
    } else if (c != runColor) {
      display_.drawVLine(col, runStart, sy - runStart, runColor);
      runStart = sy; runColor = c;
    }
  }
  if (runStart >= 0)
    display_.drawVLine(col, runStart, sy1 - runStart + 1, runColor);
}

// ---------------------------------------------------------------------------
// castColumn — the only drawing function called per column.
// Draws the full column in ONE sweep: ceiling, then (sprite OR wall), then floor.
// Gun columns get the world rendered above gun-top AND the gun slice at bottom.
// ---------------------------------------------------------------------------
void Wolf3DGame::castColumn(int col) {
  // --- DDA wall cast ---
  float cameraX = 2.0f * col / (float)kNumCols - 1.0f;
  float rayDirX = dirX_ + planeX_ * cameraX;
  float rayDirY = dirY_ + planeY_ * cameraX;
  int   mapX = (int)posX_, mapY = (int)posY_;
  float dDX = (fabsf(rayDirX)<1e-6f)?1e30f:fabsf(1.0f/rayDirX);
  float dDY = (fabsf(rayDirY)<1e-6f)?1e30f:fabsf(1.0f/rayDirY);
  float sdX, sdY; int stX, stY;
  if (rayDirX<0){stX=-1;sdX=(posX_-mapX)*dDX;}else{stX=1;sdX=(mapX+1.0f-posX_)*dDX;}
  if (rayDirY<0){stY=-1;sdY=(posY_-mapY)*dDY;}else{stY=1;sdY=(mapY+1.0f-posY_)*dDY;}
  bool hit=false,ns=false;
  while(!hit){
    if(sdX<sdY){sdX+=dDX;mapX+=stX;ns=false;}
    else       {sdY+=dDY;mapY+=stY;ns=true;}
    if(mapSolid(mapX,mapY))hit=true;
  }
  float pwd = ns?(sdY-dDY):(sdX-dDX);
  if(pwd<0.05f)pwd=0.05f;
  zBuf_[col] = pwd;

  int wS = kViewH/2 - (int)(kViewH/pwd)/2; if(wS<0) wS=0;
  int wE = kViewH/2 + (int)(kViewH/pwd)/2; if(wE>=kViewH) wE=kViewH-1;

  uint16_t wC  = wallShade(pwd, ns);
  uint16_t ceC = rgb565(30,30,60);
  uint16_t flC = rgb565(60,48,35);

  // --- Read world-sprite table (enemies) ---
  bool hasSpr = (spriteType_[col] != 0 && spriteDist_[col] < pwd);
  int  sy0    = spriteY0_[col];
  int  sy1    = spriteY1_[col];

  // --- Read gun table (gun + bullets) ---
  bool hasGun = gunCol_[col];
  int  gy0    = gunY0_[col];
  int  gy1    = gunY1_[col];

  // -----------------------------------------------------------------------
  // Draw full world column (ceiling + wall-or-sprite + floor) for all rows,
  // then overdraw the gun/bullet strip at the bottom.
  // Do NOT clip world rendering to gy0 — that caused enemy distortion at
  // the tapered gun edges where gy0 varies column-to-column.
  // The gun strip simply paints over whatever world pixels sit behind it.
  // -----------------------------------------------------------------------

  // -- Full world pass --
  if (hasSpr) {
    if (sy0 > 0) display_.drawVLine(col, 0, sy0, ceC);
    drawDemonColumn(col, sy0, sy1, spriteU_[col]);
    if (sy1 < kViewH - 1) display_.drawVLine(col, sy1 + 1, kViewH - 1 - sy1, flC);
  } else {
    if (wS > 0) display_.drawVLine(col, 0, wS, ceC);
    display_.drawVLine(col, wS, wE - wS + 1, wC);
    if (wE < kViewH - 1) display_.drawVLine(col, wE + 1, kViewH - 1 - wE, flC);
  }

  // -- Gun / bullet strip overdraw (bottom rows only) --
  if (hasGun && gy1 >= gy0)
    display_.drawVLine(col, gy0, gy1 - gy0 + 1, gunColor_[col]);
}

// ---------------------------------------------------------------------------
void Wolf3DGame::sortEnemies(int* order, float* dist) const {
  for (int i = 0; i < kNumEnemies; i++) {
    order[i] = i;
    float dx = enemies_[i].x - posX_, dy = enemies_[i].y - posY_;
    dist[i] = dx*dx + dy*dy;
  }
  for (int i = 1; i < kNumEnemies; i++) {
    int ki=order[i]; float di=dist[i]; int j=i-1;
    while (j>=0 && dist[j]<di){order[j+1]=order[j];dist[j+1]=dist[j];j--;}
    order[j+1]=ki; dist[j+1]=di;
  }
}

// ---------------------------------------------------------------------------
void Wolf3DGame::renderHud() {
  bool changed = (ammo_!=prevAmmo_)||(health_!=prevHealth_)||(score_!=prevScore_);
  if (!changed) return;
  int y = kViewH;
  display_.fillRect(0, y, kScreenW, kHudH, rgb565(15,10,10));
  int hpW = 80 * health_ / 100;
  uint16_t hpC = (health_>60)?ILI9341_GREEN:(health_>30)?ILI9341_YELLOW:ILI9341_RED;
  display_.fillRect(2, y+4, 80, 8, rgb565(40,20,20));
  display_.fillRect(2, y+4, hpW, 8, hpC);
  display_.drawText(2,   y+14, "HP",                   ILI9341_WHITE,  1, rgb565(15,10,10));
  display_.drawText(100, y+4,  "AMO "+String(ammo_),   ILI9341_YELLOW, 2, rgb565(15,10,10));
  display_.drawText(220, y+4,  "K:"+String(score_),    ILI9341_CYAN,   2, rgb565(15,10,10));
  prevAmmo_=ammo_; prevHealth_=health_; prevScore_=score_;
}

void Wolf3DGame::renderGameOver() {
  display_.fillRect(30, kViewH/2-50, kScreenW-60, 100, rgb565(10,0,0));
  bool won = (score_ >= kNumEnemies);
  display_.drawText(50, kViewH/2-30,
                    won?"YOU WIN!":"YOU DIED",
                    won?ILI9341_YELLOW:ILI9341_RED, 2, rgb565(10,0,0));
  display_.drawText(30, kViewH/2,    "Kills: "+String(score_), ILI9341_WHITE,    2, rgb565(10,0,0));
  display_.drawText(30, kViewH/2+28, "A: Restart",             ILI9341_DARKCYAN, 1, rgb565(10,0,0));
}
