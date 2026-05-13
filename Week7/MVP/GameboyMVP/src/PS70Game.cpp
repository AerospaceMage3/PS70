#include "PS70Game.h"
#include <math.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Map  (20 × 20)
// Legend:
//   1 = full-height grey wall (border)
//   2 = half-height brown table
//   3 = half-height grey station
//   0 = open floor
// ---------------------------------------------------------------------------
// clang-format off
static const uint8_t kMap[PS70Game::kMapH][PS70Game::kMapW] = {
// col: 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
/*  0 */ {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
/*  1 */ {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
/*  2 */ {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1},
/*  3 */ {1, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 1},
/*  4 */ {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1},
/*  5 */ {1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1},
/*  6 */ {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1},
/*  7 */ {1, 0, 2, 2, 2, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 2, 2, 2, 0, 1},
/*  8 */ {1, 0, 2, 0, 2, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 2, 0, 2, 0, 1},
/*  9 */ {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1},
/* 10 */ {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
/* 11 */ {1, 0, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 2, 2, 2, 0, 1},
/* 12 */ {1, 0, 2, 0, 2, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 2, 0, 2, 0, 1},
/* 13 */ {1, 0, 2, 2, 2, 0, 0, 3, 0, 0, 0, 0, 3, 0, 0, 2, 2, 2, 0, 1},
/* 14 */ {1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 1},
/* 15 */ {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1},
/* 16 */ {1, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 1},
/* 17 */ {1, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 1},
/* 18 */ {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
/* 19 */ {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
// clang-format on

// ---------------------------------------------------------------------------
// Colour helpers  (free function — used by shading tables below)
// ---------------------------------------------------------------------------
static inline uint16_t c565(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint16_t)(r & 0xF8) << 8) | ((uint16_t)(g & 0xFC) << 3) | (b >> 3);
}

// member wrapper forwards to free function (header declares it inline)
uint16_t PS70Game::wallShade(float dist, bool ns) {
  if (dist < 0.5f)  dist = 0.5f;
  if (dist > 20.0f) dist = 20.0f;
  int b = 7 - (int)(dist / 20.0f * 8.0f);
  if (b < 0) b = 0;
  if (ns && b > 0) b--;
  static const uint16_t pal[8] = {
    c565(40,40,44),  c565(65,65,72),   c565(90,90,100),  c565(115,115,128),
    c565(140,140,155),c565(165,165,180),c565(190,190,205),c565(215,215,228),
  };
  return pal[b];
}

static uint16_t tableShade(float dist, bool ns) {
  if (dist < 0.5f)  dist = 0.5f;
  if (dist > 20.0f) dist = 20.0f;
  int b = 7 - (int)(dist / 20.0f * 8.0f);
  if (b < 0) b = 0;
  if (ns && b > 0) b--;
  static const uint16_t pal[8] = {
    c565(60,30,8),  c565(90,48,14),  c565(120,65,22), c565(148,82,30),
    c565(170,98,38),c565(190,112,46),c565(208,124,54),c565(224,138,64),
  };
  return pal[b];
}

static uint16_t stationShade(float dist, bool ns) {
  if (dist < 0.5f)  dist = 0.5f;
  if (dist > 20.0f) dist = 20.0f;
  int b = 7 - (int)(dist / 20.0f * 8.0f);
  if (b < 0) b = 0;
  if (ns && b > 0) b--;
  static const uint16_t pal[8] = {
    c565(30,42,38), c565(50,65,58),  c565(70,88,78),  c565(90,110,98),
    c565(108,130,116),c565(126,150,134),c565(144,168,152),c565(160,184,168),
  };
  return pal[b];
}

static const uint16_t kTableTopColor   = c565(240,210,140);  // pale wood — bright vs brown side
static const uint16_t kStationTopColor = c565(230,235,240);  // near-white — bright vs grey-green side
static const uint16_t kCeilColor       = c565(220,220,230);
static const uint16_t kFloorColor      = c565(90, 85, 80);

// ---------------------------------------------------------------------------
// Bobby silhouette 16×16
// ---------------------------------------------------------------------------
// clang-format off
static const uint8_t kBobbySil[16][16] = {
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
};

static const uint8_t kKassiaSil[16][16] = {
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
};
// clang-format on

static uint16_t bobbyPixel(int row, int col16) {
  bool eye   = (row>=2&&row<=3) && ((col16>=4&&col16<=5)||(col16>=10&&col16<=11));
  bool pupil = (row==2) && (col16==4||col16==10);
  if (pupil)      return c565(0,0,0);
  if (eye)        return c565(100,160,255);
  if (row<=4)     return c565(220,180,140);  // skin
  if (row==5)     return c565(190,155,120);  // neck
  if (row<=11)    return c565(30,80,180);    // blue shirt
  return c565(20,20,30);                     // black pants
}

static uint16_t kassiaPixel(int row, int col16) {
  bool eye   = (row>=2&&row<=3) && ((col16>=4&&col16<=5)||(col16>=10&&col16<=11));
  bool pupil = (row==2) && (col16==4||col16==10);
  if (pupil)      return c565(0,0,0);
  if (eye)        return c565(80,140,255);
  if (row<=1)     return c565(100,60,20);    // brown hair
  if (row<=4)     return c565(220,180,140);  // skin
  if (row==5)     return c565(190,155,120);  // neck
  if (row<=11)    return c565(210,175,20);   // yellow shirt
  return c565(30,60,160);                    // blue pants
}

// ---------------------------------------------------------------------------
// Constructor / begin / reset
// ---------------------------------------------------------------------------
PS70Game::PS70Game(ILI9341Display& display, Joystick& joystick, Button& buttonA)
    : display_(display), joystick_(joystick), button_(buttonA),
      posX_(0), posY_(0), dirX_(0), dirY_(0), planeX_(0), planeY_(0),
      prevBtnA_(false), lastFrameMs_(0) {
  memset(spriteType_, 0, sizeof(spriteType_));
  memset(spriteDist_, 0, sizeof(spriteDist_));
  memset(zBuf_,       0, sizeof(zBuf_));
  memset(npcs_,       0, sizeof(npcs_));
}

void PS70Game::begin() { resetGame(); }

void PS70Game::resetGame() {
  posX_ =  9.5f; posY_ = 18.5f;  // south doorway, looking north
  dirX_  =  0.0f; dirY_ = -1.0f;
  planeX_= -0.66f; planeY_ = 0.0f;
  lastFrameMs_ = millis();
  prevBtnA_    = false;
  npcs_[NPC_BOBBY]  = { 5.5f,  9.5f, true, NPC_BOBBY  };  // left-centre aisle
  npcs_[NPC_KASSIA] = {13.5f,  9.5f, true, NPC_KASSIA }; // right-centre aisle
  display_.clear(ILI9341_BLACK);
  buildSpriteTable();
  renderColumns();
  renderHud();
}

// ---------------------------------------------------------------------------
// Map helpers
// ---------------------------------------------------------------------------
uint8_t PS70Game::mapCell(int mx, int my) const {
  if (mx < 0 || mx >= kMapW || my < 0 || my >= kMapH) return CELL_WALL;
  return kMap[my][mx];
}
bool PS70Game::mapSolid(int mx, int my) const {
  uint8_t c = mapCell(mx, my);
  return c == CELL_WALL || c == CELL_TABLE || c == CELL_STATION;
}
bool PS70Game::mapHalf(int mx, int my) const {
  uint8_t c = mapCell(mx, my);
  return c == CELL_TABLE || c == CELL_STATION;
}

// ---------------------------------------------------------------------------
// update / input
// ---------------------------------------------------------------------------
void PS70Game::update() {
  unsigned long now = millis();
  float dtMs = (float)(now - lastFrameMs_);
  if (dtMs < kFrameMs) { delay(2); return; }
  lastFrameMs_ = now;
  handleInput(dtMs / 1000.0f);
  buildSpriteTable();
  renderColumns();
  renderHud();
}

void PS70Game::handleInput(float dtSec) {
  Joystick::NormalizedReading joy = joystick_.readNormalized();
  float mv = kMoveSpeed * dtSec;
  float rt = kTurnSpeedRad * dtSec;

  float fwd = 0.0f;
  if (joy.x <= -kThreshold) fwd =  1.0f;
  if (joy.x >=  kThreshold) fwd = -1.0f;
  if (fwd != 0.0f) {
    float nx = posX_ + dirX_ * mv * fwd;
    float ny = posY_ + dirY_ * mv * fwd;
    if (!mapSolid((int)nx,    (int)posY_)) posX_ = nx;
    if (!mapSolid((int)posX_, (int)ny))    posY_ = ny;
  }

  float rot = 0.0f;
  if (joy.y >=  kThreshold) rot = -1.0f;
  if (joy.y <= -kThreshold) rot =  1.0f;
  if (rot != 0.0f) {
    float a = rot * rt, cs = cosf(a), sn = sinf(a);
    float odx = dirX_, opx = planeX_;
    dirX_   = odx*cs - dirY_*sn;   dirY_   = odx*sn + dirY_*cs;
    planeX_ = opx*cs - planeY_*sn; planeY_ = opx*sn + planeY_*cs;
  }
}

// ---------------------------------------------------------------------------
// Sprite projection
// ---------------------------------------------------------------------------
float PS70Game::projectSprite(float wx, float wy,
                              int& x0, int& x1, int& y0, int& y1) const {
  float sx = wx - posX_, sy = wy - posY_;
  float det = planeX_*dirY_ - dirX_*planeY_;
  if (fabsf(det) < 1e-6f) return 0.0f;
  float inv = 1.0f / det;
  float tX  = inv * ( dirY_*sx - dirX_*sy);
  float tY  = inv * (-planeY_*sx + planeX_*sy);
  if (tY <= 0.05f) return 0.0f;
  int cx = (int)((kNumCols/2) * (1.0f + tX/tY));
  int h  = abs((int)(kViewH / tY));
  x0 = cx - h/2; x1 = cx + h/2;
  y0 = kViewH/2 - h/2; y1 = kViewH/2 + h/2;
  if (y0 < 0)       y0 = 0;
  if (y1 >= kViewH) y1 = kViewH - 1;
  return tY;
}

void PS70Game::submitNPC(int x0, int x1, int y0, int y1, float tY, int npcType) {
  int sprW = x1 - x0 + 1;
  if (sprW <= 0) return;
  for (int x = x0; x <= x1; x++) {
    if (x < 0 || x >= kNumCols) continue;
    if (spriteType_[x] != 0 && tY >= spriteDist_[x]) continue;
    spriteType_[x] = (uint8_t)(npcType + 1);
    spriteDist_[x] = tY;
    spriteY0_[x]   = (int16_t)y0;
    spriteY1_[x]   = (int16_t)y1;
    spriteU_[x]    = (uint8_t)(255 * (x - x0) / sprW);
  }
}

void PS70Game::buildSpriteTable() {
  memset(spriteType_, 0, sizeof(spriteType_));
  memset(spriteDist_, 0, sizeof(spriteDist_));
  int order[kNumNPCs]; float dist[kNumNPCs];
  sortNPCs(order, dist);
  for (int si = 0; si < kNumNPCs; si++) {
    int i = order[si];
    if (!npcs_[i].active) continue;
    int x0,x1,y0,y1;
    float tY = projectSprite(npcs_[i].x, npcs_[i].y, x0,x1,y0,y1);
    if (tY > 0.0f) submitNPC(x0,x1,y0,y1,tY,npcs_[i].type);
  }
}

void PS70Game::sortNPCs(int* order, float* dist) const {
  for (int i = 0; i < kNumNPCs; i++) {
    order[i] = i;
    float dx = npcs_[i].x - posX_, dy = npcs_[i].y - posY_;
    dist[i]  = dx*dx + dy*dy;
  }
  // insertion sort far→near
  for (int i = 1; i < kNumNPCs; i++) {
    int ki = order[i]; float di = dist[i]; int j = i-1;
    while (j >= 0 && dist[j] < di) { order[j+1]=order[j]; dist[j+1]=dist[j]; j--; }
    order[j+1]=ki; dist[j+1]=di;
  }
}

// ---------------------------------------------------------------------------
// Sprite column drawing (run-length batched drawVLine calls)
// ---------------------------------------------------------------------------
static void drawSpriteCol(ILI9341Display& disp,
                          const uint8_t sil[16][16],
                          uint16_t (*pixFn)(int,int),
                          int col, int sy0, int sy1, uint8_t u) {
  int sprH = sy1 - sy0 + 1;
  if (sprH <= 0) return;
  int sCol = (int)u * 16 / 256;
  if (sCol > 15) sCol = 15;

  uint16_t runColor = 0;
  int      runStart = -1;
  for (int sy = sy0; sy <= sy1; sy++) {
    int sRow = (sy - sy0) * 16 / sprH;
    if (sRow > 15) sRow = 15;
    if (!sil[sRow][sCol]) {
      if (runStart >= 0) { disp.drawVLine(col, runStart, sy-runStart, runColor); runStart=-1; }
      continue;
    }
    uint16_t c = pixFn(sRow, sCol);
    if (runStart < 0) { runStart=sy; runColor=c; }
    else if (c != runColor) { disp.drawVLine(col, runStart, sy-runStart, runColor); runStart=sy; runColor=c; }
  }
  if (runStart >= 0) disp.drawVLine(col, runStart, sy1-runStart+1, runColor);
}

void PS70Game::drawBobbyColumn(int col, int sy0, int sy1, uint8_t u) {
  drawSpriteCol(display_, kBobbySil, bobbyPixel, col, sy0, sy1, u);
}
void PS70Game::drawKassiaColumn(int col, int sy0, int sy1, uint8_t u) {
  drawSpriteCol(display_, kKassiaSil, kassiaPixel, col, sy0, sy1, u);
}

// ---------------------------------------------------------------------------
// Rendering
// ---------------------------------------------------------------------------
void PS70Game::renderColumns() {
  for (int col = 0; col < kNumCols; col++) castColumn(col);
}

// castColumn:
//   Two-pass DDA:
//     Pass 1 — march through half-walls, stop only at CELL_WALL.
//              This gives the far-wall distance used for ceiling/floor/wall.
//     Pass 2 — re-march from origin, record the FIRST half-wall hit (if any)
//              that is closer than the far wall.
//   Draw order:
//     1. Full-height far wall (ceiling + wall strip + floor).
//     2. Overdraw the half-wall side strip + top face on top of the far wall.
//     3. Overdraw any NPC sprite closer than the far wall.
//   This way the ceiling above the half-wall always shows the far wall behind it.
void PS70Game::castColumn(int col) {
  float camX = 2.0f * col / (float)kNumCols - 1.0f;
  float rdX  = dirX_ + planeX_ * camX;
  float rdY  = dirY_ + planeY_ * camX;

  float ddX = (fabsf(rdX) < 1e-6f) ? 1e30f : fabsf(1.0f / rdX);
  float ddY = (fabsf(rdY) < 1e-6f) ? 1e30f : fabsf(1.0f / rdY);
  int   stX, stY;

  // ── Pass 1: find the full CELL_WALL behind everything ────────────────────
  {
    int   mX = (int)posX_, mY = (int)posY_;
    float sdX, sdY;
    if (rdX < 0) { stX=-1; sdX=(posX_-mX)*ddX; } else { stX=1; sdX=(mX+1.0f-posX_)*ddX; }
    if (rdY < 0) { stY=-1; sdY=(posY_-mY)*ddY; } else { stY=1; sdY=(mY+1.0f-posY_)*ddY; }
    bool ns = false;
    int  hitX = mX, hitY = mY;
    for (int g = 0; g < (kMapW + kMapH); g++) {
      if (sdX < sdY) { sdX += ddX; mX += stX; ns = false; }
      else           { sdY += ddY; mY += stY; ns = true;  }
      if (mX < 0 || mX >= kMapW || mY < 0 || mY >= kMapH) break;
      uint8_t cell = kMap[mY][mX];
      if (cell == CELL_WALL) { hitX=mX; hitY=mY;
        float d = ns ? (sdY-ddY) : (sdX-ddX);
        if (d < 0.05f) d = 0.05f;
        // store for use below
        zBuf_[col] = d;
        // draw far wall now — ceiling + wall + floor
        int wallH = (int)(kViewH / d);
        int wS = kViewH/2 - wallH/2; if (wS < 0)      wS = 0;
        int wE = kViewH/2 + wallH/2; if (wE >= kViewH) wE = kViewH-1;
        uint16_t wC = wallShade(d, ns);
        if (wS > 0)        display_.drawVLine(col, 0,    wS,          kCeilColor);
        display_.drawVLine(col, wS,   wE-wS+1,     wC);
        if (wE < kViewH-1) display_.drawVLine(col, wE+1, kViewH-1-wE, kFloorColor);
        break;
      }
      // skip half-walls and floors — keep marching
    }
  }

  float farDist = zBuf_[col];

  // ── Pass 2: find first half-wall closer than farDist ─────────────────────
  bool  hasHalf  = false;
  float halfDist = farDist;
  bool  halfNs   = false;
  int   halfCX = 0, halfCY = 0;
  {
    int   mX = (int)posX_, mY = (int)posY_;
    float sdX, sdY;
    if (rdX < 0) { stX=-1; sdX=(posX_-mX)*ddX; } else { stX=1; sdX=(mX+1.0f-posX_)*ddX; }
    if (rdY < 0) { stY=-1; sdY=(posY_-mY)*ddY; } else { stY=1; sdY=(mY+1.0f-posY_)*ddY; }
    bool ns = false;
    for (int g = 0; g < (kMapW + kMapH); g++) {
      if (sdX < sdY) { sdX += ddX; mX += stX; ns = false; }
      else           { sdY += ddY; mY += stY; ns = true;  }
      if (mX < 0 || mX >= kMapW || mY < 0 || mY >= kMapH) break;
      uint8_t cell = kMap[mY][mX];
      if (cell == CELL_WALL) break;  // reached the far wall, stop
      if (cell == CELL_TABLE || cell == CELL_STATION) {
        float d = ns ? (sdY-ddY) : (sdX-ddX);
        if (d < 0.05f) d = 0.05f;
        if (d < farDist) {
          hasHalf = true; halfDist = d; halfNs = ns;
          halfCX = mX; halfCY = mY;
        }
        break;
      }
    }
  }

  // ── Overdraw half-wall side strip + top face ──────────────────────────────
  if (hasHalf) {
    uint8_t  cell  = mapCell(halfCX, halfCY);
    uint16_t sideC = (cell==CELL_TABLE) ? tableShade(halfDist,halfNs) : stationShade(halfDist,halfNs);
    uint16_t topC  = (cell==CELL_TABLE) ? kTableTopColor              : kStationTopColor;

    // Half-wall sits on the floor; top is at the mid-point of a full wall.
    // fullH = pixel height of a full wall at this distance.
    // hTop = screen row of the top of the half-wall = kViewH/2 (mid of full strip)
    // hBot = bottom of full wall at this distance
    int fullH = (int)(kViewH / halfDist);
    int hTop  = kViewH/2;                          // top of half-wall = horizon of full wall
    int hBot  = kViewH/2 + fullH/2; if (hBot >= kViewH) hBot = kViewH-1;

    // Only draw if hBot > hTop (visible strip exists)
    if (hBot > hTop) {
      int topH = (hBot - hTop >= 4) ? 2 : 1;  // 2-px top band when tall enough
      display_.drawVLine(col, hTop,        topH,             topC);
      if (hBot > hTop + topH)
        display_.drawVLine(col, hTop+topH, hBot-hTop-topH,   sideC);
    }
    // Floor below the half-wall bottom is already drawn by the far-wall pass.
  }

  // ── Overdraw NPC sprite if closer than far wall ───────────────────────────
  if (spriteType_[col] != 0 && spriteDist_[col] < farDist) {
    int sy0 = spriteY0_[col], sy1 = spriteY1_[col];
    if (sy0 > 0)             display_.drawVLine(col, 0,      sy0,          kCeilColor);
    if (spriteType_[col]==1) drawBobbyColumn (col, sy0, sy1, spriteU_[col]);
    else                     drawKassiaColumn(col, sy0, sy1, spriteU_[col]);
    if (sy1 < kViewH-1)      display_.drawVLine(col, sy1+1, kViewH-1-sy1, kFloorColor);
  }
}

// ---------------------------------------------------------------------------
void PS70Game::renderHud() {
  int y = kViewH;
  uint16_t bg = c565(20,20,28);
  display_.fillRect(0, y, kScreenW, kHudH, bg);
  display_.drawText(4,   y+3, "PS70 Electronics Lab", ILI9341_CYAN,     1, bg);
  display_.drawText(210, y+3, "B:Menu",               ILI9341_DARKCYAN, 1, bg);
}
