import ModelViewer from '../../components/ModelViewer'

function Week11() {
  return (
    <div className="week-doc-content">

      {/* ── HERO ───────────────────────────────────────────── */}
      <section className="doc-section">
        <div style={{
          textAlign: 'center',
          padding: '1rem 0 1.5rem',
          borderBottom: '2px solid #ffd700',
          marginBottom: '2rem',
        }}>
          <div style={{
            fontFamily: 'var(--font-arcade)',
            fontSize: '0.55rem',
            color: '#ffd700',
            letterSpacing: '4px',
            textShadow: '0 0 12px #ffd700, 0 0 24px #ffd700',
            marginBottom: '0.5rem',
          }}>
            ★ FINAL BOSS UNLOCKED ★
          </div>
          <div style={{
            fontFamily: 'var(--font-arcade)',
            fontSize: '0.42rem',
            color: 'var(--neon-pink)',
            letterSpacing: '2px',
          }}>
            ALL STAGES CLEAR — CREDITS ROLL
          </div>
        </div>

        <div className="image-row single">
          <div className="image-card">
            <img src="images/week11/main.jpg" alt="Completed Gameman handheld" />
            <span className="image-caption">The completed Gameman — six-game handheld built from scratch</span>
          </div>
        </div>

        <div className="text-box">
          <p>
            The final project is <strong>Gameman</strong> — a fully custom handheld game console built on an
            ESP32-C3 microcontroller driving a 240×320 ILI9341 TFT display, with an analog joystick, two
            tactile buttons, a 3.4V LiPo battery, and a dedicated power switch as the only controls and power. It runs six games entirely from scratch: Snake, Dodge, Tetris,
            Tanky, and a retro inspired 3D raycaster I called "DOOM?". The whole firmware is written in C++
            using the Arduino framework via PlatformIO, with no off-the-shelf game engine. Every rendering
            loop, physics step, and frame-pacing decision was programmed to fit inside the microcontroller's
            constraints.
          </p>
          <p>
            This page documents how the project evolved from the Week 7 MVP: a redesigned and properly-fitted
            shell, a tuned joystick feel, a boot animation, and three entirely new games that pushed what the
            hardware could do — including real-time 3D rendering.
          </p>
        </div>
      </section>

      {/* ── HARDWARE / COMPONENTS ─────────────────────────── */}
      <section className="doc-section">
        <h3 className="section-title" style={{ color: '#ffd700', textShadow: '0 0 12px #ffd700' }}>
          Hardware &amp; Components
        </h3>


        <div style={{ display: 'grid', gridTemplateColumns: 'repeat(auto-fill, minmax(220px, 1fr))', gap: '1rem', marginBottom: '1.5rem' }}>
          {[
            { label: 'SEEED XIAO ESP32-C3', detail: 'RISC-V core @ 160 MHz, ~400 KB SRAM', color: 'var(--neon-green)' },
            { label: 'ILI9341 TFT 2.8"', detail: '240×320 px, RGB565 color, hardware SPI at 40 MHz. ', color: 'var(--neon-cyan)' },
            { label: 'Analog Joystick', detail: 'Two-axis potentiometer on A0/A1. Auto-calibrates center on boot; continuous min/max tracking keeps movement crisp.', color: 'var(--neon-pink)' },
            { label: 'Button A (D7)', detail: 'Select in menu, action in games. Edge-detected to avoid repeat triggers. Debounced in software.', color: 'var(--neon-yellow)' },
            { label: 'Button B (D2)', detail: 'Universal "back to menu" button. Checked before every game update so it is always responsive.', color: '#aaaaff' },
            { label: '3.4V LiPo Battery', detail: 'Rechargeable lithium polymer cell powers the device without any external cable. Wired through the XIAO\'s onboard charging circuit.', color: '#ff6600' },
            { label: 'Power Switch', detail: 'Tactile toggle on the battery line lets you fully cut power — no more unplugging USB to turn it off. Mounted flush in the shell.', color: 'var(--neon-pink)' },
            { label: 'PlatformIO + Arduino', detail: 'Build system targeting seeed_xiao_esp32c3. Adafruit GFX and Adafruit ILI9341 libs declared in platformio.ini.', color: 'var(--neon-purple)' },
          ].map(({ label, detail, color }) => (
            <div key={label} style={{
              background: 'rgba(0,0,0,0.5)',
              border: `2px solid ${color}`,
              borderRadius: '4px',
              padding: '1rem',
              boxShadow: `0 0 12px ${color}33`,
            }}>
              <div style={{ fontFamily: 'var(--font-arcade)', fontSize: '0.45rem', color, marginBottom: '0.6rem', letterSpacing: '1px' }}>
                {label}
              </div>
              <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1rem', lineHeight: 1.6, color: 'var(--text-primary)' }}>
                {detail}
              </div>
            </div>
          ))}
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Pin map &amp; hardware singletons (main.cpp)</span>
          </div>
          <pre><code>{`constexpr uint8_t JOYSTICK_X_PIN        = A0;
constexpr uint8_t JOYSTICK_Y_PIN        = A1;
constexpr uint8_t CONTROLLER_BUTTON_A_PIN = D7;
constexpr uint8_t CONTROLLER_BUTTON_B_PIN = D2;
constexpr uint8_t DISPLAY_SCK_PIN       = D8;
constexpr uint8_t DISPLAY_MOSI_PIN      = D10;
constexpr uint8_t DISPLAY_LED_PIN       = D4;
constexpr uint8_t DISPLAY_DC_PIN        = D5;
constexpr uint8_t DISPLAY_CS_PIN        = D6;
constexpr uint8_t DISPLAY_RST_PIN       = D3;

// D8/D10 must stay on the XIAO's hardware SPI peripheral.
// The 3-pin Adafruit_ILI9341(cs, dc, rst) constructor uses it automatically
// at 40 MHz — switching to the 6-pin software bit-bang path is 10-50x slower.
Joystick      joystick(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, 4095, 50);
Button        controllerButtonA(CONTROLLER_BUTTON_A_PIN);
Button        controllerButtonB(CONTROLLER_BUTTON_B_PIN);
ILI9341Display display(DISPLAY_SCK_PIN, DISPLAY_MOSI_PIN, DISPLAY_LED_PIN,
                        DISPLAY_DC_PIN,  DISPLAY_CS_PIN,  DISPLAY_RST_PIN);`}</code></pre>
        </div>

        <div className="text-box" style={{ borderLeftColor: 'var(--neon-pink)' }}>
          <p>
            A few hardware lessons learned the hard way. The battery initially refused to power the system
            reliably — after a frustrating debugging session the culprit turned out to be voltage: the LiPo
            was wired to the XIAO's 5V rail, which the battery simply can't supply. Switching everything over
            to the 3.3V output fixed it immediately. The power switch also gave me trouble in the shell — getting
            it to sit perfectly flush required multiple reprints of that section of the casing, since even a
            fraction of a millimeter off left it either sunken or proud of the surface. Finally, I wanted to add
            a small speaker for sound effects, but nothing in the lab was small enough to fit the enclosure
            without blowing up the form factor, so audio didn't make the cut.
          </p>
        </div>
      </section>

      {/* ── FORM FACTOR EVOLUTION ─────────────────────────── */}
      <section className="doc-section">
        <h3 className="section-title" style={{ color: '#ffd700', textShadow: '0 0 12px #ffd700' }}>
          Form Factor: From MVP to Final
        </h3>

        <div className="text-box">
          <p>
            The Week 7 MVP proved the concept but the shell was rough: screw holes were undersized or
            misaligned, components had to be force-fit, and the wiring was chaotic inside. For the final
            version I redesigned the shell from scratch in Fusion 360, measuring each component precisely
            and modeling screw boss geometry that actually matched M3 hardware. The back panel now seats
            flush and fastens without flexing. The most important factor in reducing size was using a solderable
            breadboard for the wiring, which also improved reliablility.
          </p>
          <p>
            Beyond structural fit, the other big mechanical improvement was joystick feel. In the MVP the
            joystick threshold and normalization were tuned too loosely, making menu navigation feel sluggish.
            The final firmware uses a deadzone calibrated at boot (<code>calibrateCenter()</code> averages 16
            samples with a 250ms settle delay), continuous min/max range tracking, and a threshold of ~0.45–0.55
            across all games — giving a crisp, predictable stick feel in every direction.
          </p>
        </div>

        <div className="image-row single">
          <div className="image-card">
            <img src="images/week11/shellevolution.jpg" alt="Shell evolution from MVP to final" />
            <span className="image-caption">Shell evolution — rough MVP prototype (left) to precision-fit final enclosure (right)</span>
          </div>
        </div>

        <div className="viewer-section" style={{ marginTop: '1.5rem' }}>
          <ModelViewer
            mode="stl"
            stlUrl="models/Gameboy1.stl"
            color={0xffffff}
            caption="Gameman final shell — drag to rotate, scroll to zoom"
          />
        </div>

        <div className="code-block" style={{ marginTop: '1.5rem' }}>
          <div className="code-header">
            <span className="code-lang">C++ — Joystick auto-calibration &amp; normalization (Joystick.h / .cpp)</span>
          </div>
          <pre><code>{`// Called once in setup() after a 250ms settle delay.
// 16 samples gives ample averaging without blocking too long.
void Joystick::calibrateCenter(uint8_t samples) {
  long xSum = 0, ySum = 0;
  for (uint8_t i = 0; i < samples; ++i) {
    xSum += readAxisAveraged(xPin_);
    ySum += readAxisAveraged(yPin_);
    delay(2);
  }
  centerX_ = xSum / samples;
  centerY_ = ySum / samples;
}

// Continuous min/max tracking means the full throw of the stick
// is always mapped to [-1, 1] regardless of unit-to-unit variation.
float Joystick::normalizeAxis(int raw, int center,
                               int minObserved, int maxObserved) const {
  int delta = raw - center;
  if (abs(delta) <= deadzone_) return 0.0f;
  int positiveRange = max(maxObserved - center, 1);
  int negativeRange = max(center - minObserved, 1);
  return constrain(
    delta > 0
      ? (float)delta / (float)positiveRange
      : (float)delta / (float)negativeRange,
    -1.0f, 1.0f);
}`}</code></pre>
        </div>
      </section>

      {/* ── BOOT ANIMATION + MENU ─────────────────────────── */}
      <section className="doc-section">
        <h3 className="section-title" style={{ color: '#ffd700', textShadow: '0 0 12px #ffd700' }}>
          Boot Animation &amp; Game Menu
        </h3>

        <div className="text-box">
          <p>
            One of the first software additions over the MVP was a startup animation. When the device
            powers on, a cyan scanline wipes down the display, then the title "GAMEMAN" materializes
            letter by letter with a flash effect, a glowing underline sweeps in, and a subtitle slides
            up from below. A border sparkles three times and a blinking "Press A to Start" prompt waits
            for input (or a 4-second timeout). The whole sequence finishes with a white flash before
            handing off to the main menu — styled to feel like powering on a real cartridge-based console.
          </p>
          <p>
            The main menu lists five games and uses the joystick X-axis to scroll the cursor, with a
            180 ms debounce interval to prevent over-scrolling. Button A launches the selected game;
            Button B anywhere during gameplay immediately returns to the menu.
          </p>
        </div>

        <div className="image-row single">
          <div className="image-card">
            <img src="images/week11/selection.jpg" alt="Game selection menu on device" />
            <span className="image-caption">The main menu running on hardware — five games selectable via joystick</span>
          </div>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Boot animation phases (main.cpp: drawStartupAnimation)</span>
          </div>
          <pre><code>{`void drawStartupAnimation() {
  // Phase 1: cyan scanline wipe top-to-bottom with trailing fade
  for (int16_t y = 0; y < H; y += 4) {
    display.fillRect(0, y, W, 4, ILI9341_CYAN);       // leading edge
    if (y > 12) display.fillRect(0, y-12, W, 8, rgb565(0,48,64));  // trail
    if (y >  4) display.fillRect(0, y- 4, W, 4, rgb565(0,80,100));
    delay(6);
  }
  display.fillRect(0, 0, W, H, rgb565(0, 8, 20));     // settle to dark bg

  // Phase 2: title materializes letter by letter with cyan flash → white
  for (int i = 0; i < 7; i++) {
    display.drawText(titleX + i*charW, titleY, &title[i], ILI9341_CYAN, 4, bg);
    delay(60);
    display.drawText(titleX + i*charW, titleY, &title[i], ILI9341_WHITE, 4, bg);
  }

  // Phase 3: glowing underline sweeps in pixel-by-pixel
  for (int16_t x = titleX; x < titleX + titleTotalW; x += 6) {
    display.fillRect(x, ulY, 6, 3, ILI9341_CYAN);
    delay(12);
  }

  // Phase 4: subtitle slides up from off-screen
  for (int16_t y = H+10; y > subTargetY; y -= 6) {
    display.fillRect(subX, max(y-6, subTargetY), subW, subSize*8+12, bg);
    display.drawText(subX, y, "A PS70 Project", rgb565(160,200,220), 2, bg);
    delay(14);
  }

  // Phase 5: border sparkle — 3 alternating cyan / blue flashes
  for (int flash = 0; flash < 3; flash++) {
    uint16_t col = (flash%2==0) ? ILI9341_CYAN : rgb565(80,160,255);
    display.drawRect(2, 2, W-4, H-4, col);
    delay(120);
    display.drawRect(2, 2, W-4, H-4, bg);
    delay(80);
  }

  // Phase 6: blinking "Press A to Start" — 4s timeout or button press
  while (millis() < deadline) {
    if (controllerButtonA.isPressed() && !prevA) break;
    if (millis() - lastBlink >= 400) {
      visible = !visible;
      display.drawText(promptX, promptY, "Press A to Start",
                       visible ? ILI9341_YELLOW : bg, 1, bg);
      lastBlink = millis();
    }
    delay(20);
  }
  display.fillRect(0, 0, W, H, ILI9341_WHITE);  // white flash → menu
}`}</code></pre>
        </div>
      </section>

      {/* ── NEW GAMES ─────────────────────────────────────── */}
      <section className="doc-section">
        <h3 className="section-title" style={{ color: '#ffd700', textShadow: '0 0 12px #ffd700' }}>
          New Games — Built From Scratch With Claude Code
        </h3>

        <div className="text-box">
          <p>
            The MVP shipped two games: Snake and Dodge. For the final project I added three more, each built
            entirely from scratch in C++ (with the help of Claude Code). Every
            rendering loop, collision system, and game-state machine was written specifically for this
            hardware with no game libraries, no sprite engines, and no framebuffers. Each game borrows the same
            display, joystick, and button references from <code>main.cpp</code> and exposes only
            <code>begin()</code> and <code>update()</code>.
          </p>
        </div>

        {/* Tetris */}
        <div style={{
          background: 'rgba(0,0,0,0.4)',
          border: '2px solid var(--neon-cyan)',
          borderRadius: '6px',
          padding: '1.25rem',
          marginBottom: '1.5rem',
        }}>
          <div style={{ fontFamily: 'var(--font-arcade)', fontSize: '0.7rem', color: 'var(--neon-cyan)', marginBottom: '0.75rem', textShadow: '0 0 10px var(--neon-cyan)' }}>
            TETRIS
          </div>
          <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1.1rem', lineHeight: 1.7, color: 'var(--text-primary)', marginBottom: '1rem' }}>
            Standard 7-tetromino Tetris on a 10×20 board (11 px/cell). Joystick Y-axis moves left/right;
            joystick X-axis (pushed down) soft-drops; Button A rotates CW with wall-kick. Score follows
            the real line-clear table (100/300/500/800 × level). Level increases every 10 lines and fall
            speed decreases by 40 ms per level, floored at 100 ms. A next-piece preview and score/lines/level
            sidebar render on the right. All 7 tetromino shapes are encoded as 4-rotation tables in a
            single static array — no heap allocation, no sprite sheets.
          </div>
          <div className="code-block">
            <div className="code-header">
              <span className="code-lang">C++ — 7-tetromino rotation table (TetrisGame.cpp)</span>
            </div>
            <pre><code>{`// 7 pieces × 4 rotations × 4 cells × {row_offset, col_offset}
// All encoded statically — no heap, no sprite sheets.
const int8_t TetrisGame::kPieces[7][4][4][2] = {
  // I (cyan) — horizontal bar, symmetric across 2 rotations
  {{{-1,0},{0,0},{1,0},{2,0}}, {{0,-1},{0,0},{0,1},{0,2}}, ...},
  // O (yellow) — 2×2 square, rotation-invariant
  {{{0,0},{0,1},{1,0},{1,1}}, {{0,0},{0,1},{1,0},{1,1}}, ...},
  // T (purple)
  {{{0,-1},{0,0},{0,1},{-1,0}}, {{-1,0},{0,0},{1,0},{0,1}}, ...},
  // S, Z, J, L ...
};

const uint16_t TetrisGame::kColors[7] = {
  0x07FF,  // I: cyan
  0xFFE0,  // O: yellow
  0x801F,  // T: purple
  0x07E0,  // S: green
  0xF800,  // Z: red
  0x001F,  // J: blue
  0xFD20,  // L: orange
};`}</code></pre>
          </div>
        </div>

        {/* Tanky */}
        <div style={{
          background: 'rgba(0,0,0,0.4)',
          border: '2px solid var(--neon-green)',
          borderRadius: '6px',
          padding: '1.25rem',
          marginBottom: '1.5rem',
        }}>
          <div style={{ fontFamily: 'var(--font-arcade)', fontSize: '0.7rem', color: 'var(--neon-green)', marginBottom: '0.75rem', textShadow: '0 0 10px var(--neon-green)' }}>
            TANKY
          </div>
          <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1.1rem', lineHeight: 1.7, color: 'var(--text-primary)', marginBottom: '1rem' }}>
            Top-down tank battle filling the full 288×240 arena. Joystick Y-axis rotates the player tank;
            joystick X-axis drives forward/backward. Button A fires with a 600 ms cooldown. An AI tank
            tracks and engages the player each round — hit it to score and start the next round, or get hit
            to end the game. Rather than clearing and redrawing the whole screen every frame (which causes
            visible flicker on an SPI display), the renderer only repaints the parts that actually moved:
            <code>eraseTank</code> / <code>drawTank</code> only fire when a tank's position or angle changed
            since the last frame, and the trig values for that angle are cached and reused until the angle
            changes again. Everything else on screen stays untouched.
          </div>
          <div className="code-block">
            <div className="code-header">
              <span className="code-lang">C++ — Tank state &amp; dirty-flag incremental redraw (TankyGame.cpp)</span>
            </div>
            <pre><code>{`// Tanks are placed at opposite ends facing each other.
// sinA/cosA are cached and recomputed only when angle changes.
void TankyGame::setAngle(Tank& t, int16_t deg) {
  if (t.angleDeg == deg) return;
  t.angleDeg = deg;
  t.sinA = sinDeg(deg);
  t.cosA = cosDeg(deg);
}

// In update() — only redraw if position or angle changed.
if (player_.x != player_.prevX || player_.angleDeg != player_.prevAngle) {
  eraseTank(player_);
  drawTank(player_);
  player_.prevX    = player_.x;
  player_.prevAngle = player_.angleDeg;
}

// Arena edge only drawn on full redraws — tanks are clamped
// inside the wall so the edge is never dirtied per-frame.
if (needsFullRedraw_) {
  drawArenaEdge();
  needsFullRedraw_ = false;
}`}</code></pre>
          </div>
        </div>

        {/* Wolf3D */}
        <div style={{
          background: 'rgba(0,0,0,0.4)',
          border: '2px solid var(--neon-pink)',
          borderRadius: '6px',
          padding: '1.25rem',
          marginBottom: '1.5rem',
        }}>
          <div style={{ fontFamily: 'var(--font-arcade)', fontSize: '0.7rem', color: 'var(--neon-pink)', marginBottom: '0.75rem', textShadow: '0 0 10px var(--neon-pink)' }}>
            DOOM? — WOLFENSTEIN-STYLE 3D RAYCASTER
          </div>
          <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1.1rem', lineHeight: 1.7, color: 'var(--text-primary)', marginBottom: '1rem' }}>
            The most technically ambitious game: a DOOM-style raycaster rendering a first-person 3D
            view entirely in software on the ESP32-C3. Each frame, one ray is cast per screen column using
            DDA (digital differential analysis) to find wall intersections, and the wall strip height is
            computed from the perpendicular distance. Six demon enemies stalk the player on a 16×16 map;
            kill all six to win, reach 0 HP to lose. Joystick X moves forward/back, joystick Y turns.
            Button A fires with a 400 ms cooldown and 40 ammo.
          </div>
          <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1.1rem', lineHeight: 1.7, color: 'var(--text-primary)', marginBottom: '1rem' }}>
            The rendering pipeline runs two independent per-column tables each frame — one for enemy sprites,
            one for the gun geometry and bullets. The final column draw sweeps in a single pass: ceiling,
            enemy silhouette or wall strip, floor, then gun/bullet overdraw at the bottom. This separation
            prevents the enemy-distortion artifact that occurred when the gun's tapered top edge was used to
            clip world rendering per-column.
          </div>
          <div style={{ fontFamily: 'var(--font-terminal)', fontSize: '1.1rem', lineHeight: 1.7, color: 'var(--text-primary)', marginBottom: '1rem' }}>
            Enemy sprites use a hand-coded 16×16 silhouette bitmap (head, shoulder bar with arm-gap cutouts,
            two leg pillars) rendered by mapping screen pixels back to silhouette row/col, batching consecutive
            same-colour opaque runs into single <code>drawVLine</code> calls. Eye highlights are composited
            on top. Wall shading uses a distance-banded 8-colour palette for depth perception.
          </div>

          <div className="image-row single" style={{ marginBottom: '1rem' }}>
            <div className="image-card">
              <img src="images/week11/3dGame.jpg" alt="DOOM? 3D raycaster running on device" />
              <span className="image-caption">DOOM? raycaster — real-time 3D rendering on the ESP32-C3</span>
            </div>
          </div>

          <div className="code-block">
            <div className="code-header">
              <span className="code-lang">C++ — 16×16 map &amp; demon silhouette bitmap (Wolf3DGame.cpp)</span>
            </div>
            <pre><code>{`// 16×16 map: 1 = wall, 0 = open. Player spawns at (1.5, 1.5).
static const uint8_t kMap[16][16] = {
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1},
  {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1},
  // ... (symmetric interior with scattered pillars)
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

// 16-column × 16-row silhouette: 1=opaque, 0=transparent.
// head (rows 0-4) → neck (5) → shoulders/arms (6-8) → torso (9-11) → legs (12-15)
static const uint8_t kDemonSil[16][16] = {
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},  // head top
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},  // head bottom
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},  // neck
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},  // shoulder bar (full width)
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},  // arm-gap cutouts
  {1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,1},
  {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},  // torso
  {0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
  {0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0},  // waist
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},  // leg gap
  {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0},
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},  // lower legs
  {0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
};

// Wall shading: 8 distance bands from dark (far) to bright (close).
uint16_t Wolf3DGame::wallShade(float dist, bool nsWall) {
  int band = 7 - (int)(min(dist, 12.0f) / 12.0f * 8.0f);
  if (nsWall && band > 0) band--;  // N/S walls slightly darker
  static const uint16_t pal[8] = {
    rgb565(32,36,40),  rgb565(55,62,68),
    rgb565(80,90,100), rgb565(105,118,130),
    rgb565(130,145,160), rgb565(160,175,190),
    rgb565(190,205,215), rgb565(220,228,235),
  };
  return pal[band];
}`}</code></pre>
          </div>

          <div className="code-block">
            <div className="code-header">
              <span className="code-lang">C++ — Enemy spawn with distance constraints (Wolf3DGame.cpp: resetGame)</span>
            </div>
            <pre><code>{`// Each of 6 enemies: up to 200 placement attempts per enemy.
// Constraints: open cell, >= 3 units from player, >= 1.5 units from other enemies.
randomSeed(micros());
for (int i = 0; i < kNumEnemies; i++) {
  float ex = 3.5f, ey = 5.5f;  // fallback position if all attempts fail
  for (int attempt = 0; attempt < 200; attempt++) {
    int mx = (int)random(1, kMapW-1);
    int my = (int)random(1, kMapH-1);
    if (mapSolid(mx, my)) continue;
    float cx = mx+0.5f, cy = my+0.5f;
    float pdx = cx-posX_, pdy = cy-posY_;
    if (pdx*pdx + pdy*pdy < 9.0f) continue;      // too close to player
    bool tooClose = false;
    for (int j = 0; j < i; j++) {
      float edx = cx-enemies_[j].x, edy = cy-enemies_[j].y;
      if (edx*edx + edy*edy < 2.25f) { tooClose=true; break; }
    }
    if (tooClose) continue;
    ex = cx; ey = cy; break;
  }
  enemies_[i] = {ex, ey, true, 1};
}`}</code></pre>
          </div>
        </div>
      </section>

      {/* ── DEMO VIDEO ────────────────────────────────────── */}
      <section className="doc-section" style={{ borderBottom: 'none' }}>
        <h3 className="section-title" style={{ color: '#ffd700', textShadow: '0 0 12px #ffd700' }}>
          Full Demo
        </h3>

        <div className="text-box">
          <p>
            Boot animation, menu navigation, and all five games running on the final hardware.
          </p>
        </div>

        <div className="video-container">
          <video controls playsInline muted>
            <source src="images/week11/finaldemo.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Full system demo — boot animation through all five games on final hardware</span>
        </div>
      </section>

    </div>
  )
}

export default Week11
