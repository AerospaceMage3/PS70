#pragma once

#include <Arduino.h>
#include "Button.h"
#include "ILI9341Display.h"
#include "Joystick.h"

class PS70Game {
 public:
  PS70Game(ILI9341Display& display, Joystick& joystick, Button& buttonA);

  void begin();
  void update();

  static constexpr int kMapW = 20;
  static constexpr int kMapH = 20;

 private:
  // ── Screen / view constants ──────────────────────────────────────────────
  static constexpr int kScreenW = 288;
  static constexpr int kScreenH = 240;
  static constexpr int kHudH    = 16;
  static constexpr int kViewH   = kScreenH - kHudH;
  static constexpr int kNumCols = kScreenW;

  // ── Movement / timing ────────────────────────────────────────────────────
  static constexpr float kMoveSpeed    = 2.0f;
  static constexpr float kTurnSpeedRad = 2.0f;
  static constexpr float kThreshold    = 0.45f;
  static constexpr int   kFrameMs      = 40;

  // ── Map cell types ────────────────────────────────────────────────────────
  // 0  = open floor
  // 1  = full-height grey wall (border)
  // 2  = half-height brown table
  // 3  = half-height grey station
  static constexpr uint8_t CELL_FLOOR   = 0;
  static constexpr uint8_t CELL_WALL    = 1;
  static constexpr uint8_t CELL_TABLE   = 2;
  static constexpr uint8_t CELL_STATION = 3;

  // ── NPC types ─────────────────────────────────────────────────────────────
  static constexpr int kNumNPCs = 2;
  static constexpr int NPC_BOBBY  = 0;
  static constexpr int NPC_KASSIA = 1;
  struct NPC { float x, y; bool active; int type; };

  // ── Sprite tables (world sprites = half-walls + NPCs) ─────────────────────
  // spriteType_: 0=none, 1=NPC Bobby, 2=NPC Kassia, 3=table top, 4=station top
  uint8_t  spriteType_[kNumCols];
  float    spriteDist_[kNumCols];
  int16_t  spriteY0_[kNumCols];
  int16_t  spriteY1_[kNumCols];
  uint8_t  spriteU_[kNumCols];   // 0–255 normalised x across sprite

  // ── z-buffer ──────────────────────────────────────────────────────────────
  float zBuf_[kNumCols];

  // ── State ─────────────────────────────────────────────────────────────────
  float posX_, posY_, dirX_, dirY_, planeX_, planeY_;
  bool  prevBtnA_;
  unsigned long lastFrameMs_;

  NPC npcs_[kNumNPCs];

  ILI9341Display& display_;
  Joystick&       joystick_;
  Button&         button_;

  // ── Internal helpers ──────────────────────────────────────────────────────
  void resetGame();
  void handleInput(float dtSec);

  bool mapSolid(int mx, int my) const;
  bool mapHalf(int mx, int my) const;
  uint8_t mapCell(int mx, int my) const;

  void buildSpriteTable();
  void renderColumns();
  void castColumn(int col);
  void renderHud();

  float projectSprite(float wx, float wy,
                      int& x0, int& x1, int& y0, int& y1) const;
  void submitNPC(int x0, int x1, int y0, int y1, float tY, int npcType);

  void drawBobbyColumn(int col, int sy0, int sy1, uint8_t u);
  void drawKassiaColumn(int col, int sy0, int sy1, uint8_t u);
  void sortNPCs(int* order, float* dist) const;

  static uint16_t wallShade(float dist, bool nsWall);
};
