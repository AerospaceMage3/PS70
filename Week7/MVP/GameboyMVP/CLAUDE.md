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

A handheld "Gameboy MVP": ESP32-C3 + ILI9341 240x320 SPI display + analog joystick + single tactile button, hosting a menu and two mini-games.

### Entry point and app state machine
[src/main.cpp](src/main.cpp) owns all hardware singletons and the top-level `AppMode` enum (`Menu`, `Snake`, `Dodge`). One `Joystick`, `Button`, and `ILI9341Display` are constructed at file scope and passed *by reference* into both game classes' constructors — games never own peripherals, they borrow them. To add a new game, follow the same pattern: take `(ILI9341Display&, Joystick&, Button&)`, expose `begin()` / `update()`, add an `AppMode` value, and wire it into `startSelectedGame()` and the dispatch in `loop()`.

### Pin map (defined in main.cpp, not a header)
Joystick X/Y → A0/A1, controller button → D7, ILI9341 SCK/MOSI/LED/DC/CS/RST → D8/D10/D4/D5/D6/D3. The display class also defaults to these pins in [include/ILI9341Display.h](include/ILI9341Display.h) — keep the two in sync if pins change.

The display uses the Adafruit hardware SPI constructor `Adafruit_ILI9341(cs, dc, rst)` at 40 MHz. D8/D10 must remain on the XIAO ESP32-C3's hardware SPI peripheral. Do **not** switch to the 5/6-pin Adafruit_ILI9341 constructor `(cs, dc, mosi, sck, rst, miso)` — that is the software bit-bang path and is ~10–50× slower.

### Universal "back to menu" gesture
Three button presses within `TRIPLE_TAP_WINDOW_MS` (3s) anywhere — including inside a game — returns to the menu. This is implemented once in `processButtonTaps()` in [src/main.cpp:85](src/main.cpp#L85), which runs *before* per-mode dispatch in `loop()`. Games therefore must not assume they have exclusive control of the button: they only see button state on frames where the triple-tap handler didn't already consume it.

### Rendering convention: dirty-flag incremental redraws
All three modes (menu, Snake, Dodge) maintain `previous*` snapshot fields and a `needsFullRedraw_` / `menuDirty` flag, then only repaint cells whose state changed since the last frame. The display has no framebuffer abstraction — drawing goes straight to the Adafruit_ILI9341 driver via thin wrappers in [include/ILI9341Display.h](include/ILI9341Display.h). When adding rendering code, follow the same diff-and-repaint approach; full `clear()` calls cause visible flicker on this SPI display.

### Joystick normalization
[include/Joystick.h](include/Joystick.h) returns `NormalizedReading` in [-1, 1] after auto-calibration (`calibrateCenter()` is called once in `setup()` after a 250ms settle delay) and continuous min/max range tracking. Code that reacts to joystick direction should compare against a threshold (~0.45–0.55 is the convention used in existing games), not raw ADC values.

`readAxisAveraged` defaults to 2 samples with no inter-sample delay — enough for per-frame input polling given the deadzone. Don't raise the sample count or add `delayMicroseconds` delays; it was previously 8 samples × 150 µs = ~2.4 ms of blocking per `readNormalized()` call. `calibrateCenter()` loops 16 times externally so it still gets ample averaging.

### Frame pacing
There is no scheduler — each `update()` is called every `loop()` iteration and gates its own work with `millis()` deltas (`kStepIntervalMs`, `kFrameIntervalMs`, `kPlayerMoveIntervalMs`, etc.). The menu loop uses an explicit `delay(20)` to throttle; games rely on their interval gates plus short `delay(20)` calls in idle/game-over branches. Avoid long `delay()` calls in active-play branches — they will block the triple-tap-to-menu gesture.
