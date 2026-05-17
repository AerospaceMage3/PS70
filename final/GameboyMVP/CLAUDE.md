# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build / Upload / Monitor

PlatformIO project targeting `seeed_xiao_esp32c3` (Arduino framework). Run from this directory:

- Build: `pio run`
- Upload to board: `pio run -t upload`
- Serial monitor (115200 baud — set in [src/main.cpp:127](src/main.cpp#L127)): `pio device monitor -b 115200`
- Clean: `pio run -t clean`

Library deps (`adafruit/Adafruit GFX`, `adafruit/Adafruit ILI9341`) are declared in [platformio.ini](platformio.ini) and fetched automatically by PlatformIO. The `test/` and `lib/` directories are stub PlatformIO scaffolding — no tests exist. `pio` lives at `~/.platformio/penv/bin/pio` — it is not on the system PATH.

## Architecture

A handheld "Gameboy MVP": ESP32-C3 + ILI9341 240x320 SPI display + analog joystick + two tactile buttons, hosting a menu and six mini-games.

### Entry point and app state machine
[src/main.cpp](src/main.cpp) owns all hardware singletons and the top-level `AppMode` enum (`Menu`, `Snake`, `Dodge`, `Tetris`, `Tanky`, `Wolf3D`, `PS70`). One `Joystick`, two `Button`s, and one `ILI9341Display` are constructed at file scope and passed *by reference* into all game classes' constructors — games never own peripherals, they borrow them. To add a new game, follow the same pattern: take `(ILI9341Display&, Joystick&, Button&)`, expose `begin()` / `update()`, add an `AppMode` value, and wire it into `startSelectedGame()` and the dispatch in `loop()`. The menu supports 6 entries (indices 0–5, clamped with `min(5, ...)`).

### Games
- **Snake** ([include/SnakeGame.h](include/SnakeGame.h), [src/SnakeGame.cpp](src/SnakeGame.cpp)): classic snake on a 16×12 grid. Joystick steers; eat food to grow.
- **Dodge** ([include/SimpleGame.h](include/SimpleGame.h), [src/SimpleGame.cpp](src/SimpleGame.cpp)): dodge falling enemies across 8 lanes. Joystick moves player.
- **Tetris** ([include/TetrisGame.h](include/TetrisGame.h), [src/TetrisGame.cpp](src/TetrisGame.cpp)): standard 7-tetromino Tetris on a 10×20 board (11 px/cell). Joystick Y-axis moves left/right; joystick X-axis (down) soft-drops; Button A rotates CW with wall-kick. Score follows the standard line-clear table (100/300/500/800 × level). Level increases every 10 lines; fall speed decreases by 40 ms/level with a 100 ms floor. Next-piece preview and score/lines/level sidebar on the right.
- **DOOM?** (`AppMode::Wolf3D`, [include/Wolf3DGame.h](include/Wolf3DGame.h), [src/Wolf3DGame.cpp](src/Wolf3DGame.cpp)): Wolfenstein-style raycaster. Joystick X-axis moves forward/back (`joy.x <= -threshold` = forward); joystick Y-axis turns left/right. Button A fires a projectile (400 ms cooldown, 40 ammo). 6 stalking demon enemies on a 16×16 map; kill all to win, reach 0 HP to lose. Enemies spawn at random open map cells each game start — seeded with `randomSeed(micros())` in `resetGame()`. Spawn constraints: ≥3 map units from player (1.5, 1.5), ≥1.5 map units from each other enemy, not inside a wall. Up to 200 retry attempts per enemy; falls back to (3.5, 5.5) if no valid cell is found.

  **Rendering pipeline** — `buildSpriteTable()` then `renderColumns()` each frame. Two independent per-column tables are populated before any drawing:
  - *World-sprite table* (`spriteType_`, `spriteDist_`, `spriteY0_/Y1_`, `spriteU_`): enemies only. type 1 = demon.
  - *Gun table* (`gunCol_`, `gunY0_/Y1_`, `gunColor_`): bullets (submitted first) and gun geometry (submitted second, skips columns already claimed by bullets). This separation is critical — the gun table never overwrites enemy data, so enemies remain visible through and above the gun.

  `castColumn()` draws each column in a single sweep: full world pass (ceiling + demon-silhouette-or-wall + floor for all rows), then overdraw the gun/bullet strip at the bottom rows. The overdraw approach avoids the enemy-distortion artifact that results from clipping world rendering to a per-column gun top (`gy0` varies as the gun tapers, which caused stepped cuts across enemy sprites).

  **Enemy sprites** use a `kDemonSil[16][16]` silhouette bitmap (circular head, shoulder bar with arm-gap cutouts, two leg pillars separated by a gap). `drawDemonColumn()` maps screen pixels → silhouette row/col, skips transparent rows, and batches consecutive same-colour opaque runs into single `drawVLine` calls. Eye highlights (yellow sclera, black pupil) are drawn on top.

  **Gun** is a perspective-foreshortened shotgun barrel: tallest at the barrel centre column (`kScreenW * 60%`), tapering outward. Bore columns show a dark circle (muzzle flash on fire). Stock/receiver extend rightward with wood/metal colours. Recoil: gun drops 10 px when idle, snaps flush on fire. Bullets spawn offset by `planeX/Y * 0.18` to align with the barrel centre.

  **Bullets** travel in world-space at `kBulletSpeed` map-units/s, rendered as tiny 3 px sprites in the gun table. Hit detection is AABB (0.3 map-unit radius). HUD: HP bar + ammo count + kill count (bottom 24 px strip). `drawVLine` wraps `Adafruit_ILI9341::drawFastVLine` for column rendering performance. Key tuning constants: `kMoveSpeed`, `kTurnSpeedRad`, `kBulletSpeed`, `kNumEnemies`.
- **Tanky** ([include/TankyGame.h](include/TankyGame.h), [src/TankyGame.cpp](src/TankyGame.cpp)): top-down tank battle. Joystick Y-axis rotates the tank; joystick X-axis drives forward/backward (`joy.x <= -threshold` = forward, `joy.x >= threshold` = backward). Button A fires a projectile (600 ms cooldown). Hit the AI tank to score and start the next round; getting hit or colliding with the AI ends the game. Arena fills the full 288×240 display area with score overlaid in the top-left corner. Key tuning constants in the header: `kTankSpeed` (px/s), `kBulletSpeed` (px/s), `kTurnSpeedDeg` (°/s, currently **240°/s** for responsive player steering). Rendering uses dirty-flag incremental redraws — `eraseTank`/`drawTank` on position/angle change only; `drawArenaEdge` is called only on full redraws (not per-frame erase) since tanks are clamped inside the wall. Tank `sinA`/`cosA` are cached and updated only when the angle changes via `setAngle()`.

### Pin map (defined in main.cpp, not a header)
Joystick X/Y → A0/A1, `controllerButtonA` (select/confirm) → D7, `controllerButtonB` (home) → D2, ILI9341 SCK/MOSI/LED/DC/CS/RST → D8/D10/D4/D5/D6/D3. The display class also defaults to these pins in [include/ILI9341Display.h](include/ILI9341Display.h) — keep the two in sync if pins change.

The display uses the Adafruit hardware SPI constructor `Adafruit_ILI9341(cs, dc, rst)` at 40 MHz. D8/D10 must remain on the XIAO ESP32-C3's hardware SPI peripheral. Do **not** switch to the 5/6-pin Adafruit_ILI9341 constructor `(cs, dc, mosi, sck, rst, miso)` — that is the software bit-bang path and is ~10–50× slower.

### Universal "back to menu" button
A single press of `controllerButtonB` (D2) anywhere — including inside a game — returns to the menu. This is handled in `processButtons()` in [src/main.cpp:86](src/main.cpp#L86), which runs *before* per-mode dispatch in `loop()`. `controllerButtonA` is used only to launch a game from the menu; games currently receive it by reference but the home button is handled exclusively in `processButtons()`.

### Joystick axis convention
The Y axis (`joy.y`) maps to left/right in landscape orientation: `joy.y >= threshold` → left, `joy.y <= -threshold` → right. The X axis (`joy.x`) maps to up/down: `joy.x >= threshold` → down. This is confirmed across Snake and Tetris — follow the same signs when adding new directional input.

### Rendering convention: dirty-flag incremental redraws
All modes (menu, Snake, Dodge, Tetris, Tanky) maintain `previous*` snapshot fields and a `needsFullRedraw_` / `menuDirty` flag, then only repaint cells whose state changed since the last frame. The display has no framebuffer abstraction — drawing goes straight to the Adafruit_ILI9341 driver via thin wrappers in [include/ILI9341Display.h](include/ILI9341Display.h). When adding rendering code, follow the same diff-and-repaint approach; full `clear()` calls cause visible flicker on this SPI display.

### Joystick normalization
[include/Joystick.h](include/Joystick.h) returns `NormalizedReading` in [-1, 1] after auto-calibration (`calibrateCenter()` is called once in `setup()` after a 250ms settle delay) and continuous min/max range tracking. Code that reacts to joystick direction should compare against a threshold (~0.45–0.55 is the convention used in existing games), not raw ADC values.

`readAxisAveraged` defaults to 2 samples with no inter-sample delay — enough for per-frame input polling given the deadzone. Don't raise the sample count or add `delayMicroseconds` delays; it was previously 8 samples × 150 µs = ~2.4 ms of blocking per `readNormalized()` call. `calibrateCenter()` loops 16 times externally so it still gets ample averaging.

### Frame pacing
There is no scheduler — each `update()` is called every `loop()` iteration and gates its own work with `millis()` deltas (`kStepIntervalMs`, `kFrameIntervalMs`, `kPlayerMoveIntervalMs`, etc.). The menu loop uses an explicit `delay(20)` to throttle; games rely on their interval gates plus short `delay(20)` calls in idle/game-over branches. Avoid long `delay()` calls in active-play branches — they will delay recognition of the home button press in `processButtons()`.
