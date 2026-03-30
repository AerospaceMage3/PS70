function Week7() {
  return (
    <div className="week-doc-content">
      <section className="doc-section">
        <h3 className="section-title">MVP: Handheld Game System</h3>
        <div className="text-box">
          <p>
            This week I built the MVP for my final project: a Gameboy-like handheld with a joystick,
            a button, and a TFT display. The goal was to prove the full interaction loop end-to-end:
            read controls, render graphics, run game logic, and package everything into a playable
            physical system.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week7/final-rotated.jpg" alt="Final handheld MVP" />
            <span className="image-caption">Final MVP handheld system</span>
          </div>
        </div>
      </section>

      <section className="doc-section">
        <h3 className="section-title">Shell Design &amp; Mechanical Issues</h3>
        <div className="text-box">
          <p>
            I started by designing a shell for the joystick, button, display, and microcontroller.
            I measured and placed screw holes as carefully as I could, but this became one of the
            biggest pain points in the build. Many holes ended up undersized or slightly misaligned,
            so I had to improvise with rework and force-fit solutions to get components to seat.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week7/printedcasing.jpg" alt="3D printed shell" />
            <span className="image-caption">First printed casing and mounting layout</span>
          </div>
        </div>
      </section>

      <section className="doc-section">
        <h3 className="section-title">Component-by-Component Wiring &amp; Code Tests</h3>
        <div className="text-box">
          <p>
            Before final assembly, I tested each subsystem independently: joystick input,
            button input, and TFT display output. I also validated pin assignments and startup
            sequencing in the main software was correct so that full integration would be
            less time-consuming.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week7/joystick.jpg" alt="Joystick test setup" />
            <span className="image-caption">Joystick module test setup</span>
          </div>
          <div className="image-card">
            <img src="images/week7/wiringcomponents.jpg" alt="Wiring components on table" />
            <span className="image-caption">Wiring each module before integration</span>
          </div>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Joystick calibration, deadzone, and normalization</span>
          </div>
          <pre><code>{`// Joystick.cpp
void Joystick::calibrateCenter(uint8_t samples) {
  long xSum = 0;
  long ySum = 0;
  for (uint8_t i = 0; i < samples; ++i) {
    xSum += readAxisAveraged(xPin_);
    ySum += readAxisAveraged(yPin_);
    delay(2);
  }
  centerX_ = static_cast<int>(xSum / samples);
  centerY_ = static_cast<int>(ySum / samples);
}

float Joystick::normalizeAxis(int raw, int center, int minObserved, int maxObserved) const {
  int delta = raw - center;
  if (abs(delta) <= deadzone_) return 0.0f;

  int positiveRange = max(maxObserved - center, 1);
  int negativeRange = max(center - minObserved, 1);
  return constrain(delta > 0
      ? static_cast<float>(delta) / static_cast<float>(positiveRange)
      : static_cast<float>(delta) / static_cast<float>(negativeRange),
      -1.0f, 1.0f);
}`}</code></pre>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Button input</span>
          </div>
          <pre><code>{`// Button.cpp
void Button::begin() {
  pinMode(pin_, INPUT_PULLUP);
}

bool Button::isPressed() const {
  return digitalRead(pin_) == LOW;
}`}</code></pre>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — TFT display bring-up</span>
          </div>
          <pre><code>{`// ILI9341Display.cpp
void ILI9341Display::begin(uint8_t rotation) {
  if (tft_ == nullptr) {
    tft_ = new Adafruit_ILI9341(csPin_, dcPin_, mosiPin_, sckPin_, rstPin_, -1);
  }

  pinMode(backlightPin_, OUTPUT);
  digitalWrite(backlightPin_, HIGH);

  tft_->begin(10000000);
  tft_->setRotation(rotation);
  tft_->fillScreen(ILI9341_BLACK);
}`}</code></pre>
        </div>

        <div className="download-row">
          <a href="/downloads/week7/ILI9341Display.cpp" download className="download-btn">
            ⬇ DOWNLOAD ILI9341Display.cpp
          </a>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Wiring map and system initialization</span>
          </div>
          <pre><code>{`// main.cpp (selected)
constexpr uint8_t JOYSTICK_X_PIN = A0;
constexpr uint8_t JOYSTICK_Y_PIN = A1;
constexpr uint8_t CONTROLLER_BUTTON_PIN = D7;
constexpr uint8_t DISPLAY_SCK_PIN = D8;
constexpr uint8_t DISPLAY_MOSI_PIN = D10;
constexpr uint8_t DISPLAY_LED_PIN = D4;
constexpr uint8_t DISPLAY_DC_PIN = D5;
constexpr uint8_t DISPLAY_CS_PIN = D6;
constexpr uint8_t DISPLAY_RST_PIN = D3;

void setup() {
  Serial.begin(115200);
  joystick.begin();
  controllerButton.begin();
  display.begin();

  delay(250);
  joystick.calibrateCenter();
  renderMenu();
}`}</code></pre>
        </div>

        <div className="download-row">
          <a href="/downloads/Week7-GameboyMVP-source.zip" download className="download-btn">
            ⬇ DOWNLOAD WEEK 7 SOURCE (ZIP)
          </a>
          <a href="/downloads/week7/main.cpp" download className="download-btn">
            ⬇ DOWNLOAD main.cpp
          </a>
          <a href="/downloads/week7/Joystick.cpp" download className="download-btn">
            ⬇ DOWNLOAD Joystick.cpp
          </a>
        </div>
      </section>

      <section className="doc-section">
        <h3 className="section-title">Full Circuit Integration (Outside the Shell)</h3>
        <div className="text-box">
          <p>
            Once each module worked on its own, I combined everything on a temporary external
            wiring setup and ran integrated tests. This stage was crucial for debugging the
            devices behavior before I had to deal with the constraints inside the enclosure.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week7/testingcircuit.jpg" alt="Integrated circuit test outside shell" />
            <span className="image-caption">End-to-end circuit and firmware test outside the casing</span>
          </div>
        </div>
      </section>

      <section className="doc-section">
        <h3 className="section-title">Final Assembly Into the Casing</h3>
        <div className="text-box">
          <p>
            After the circuit and code were stable, I installed everything into the shell.
            Internal space was tight, and I had to force and route wiring carefully to make
            everything fit without disconnecting components. This was definitely the most
            physically frustrating stage, but it got the device into a functional handheld form.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week7/installingcomponents.jpg" alt="Installing components into shell" />
            <span className="image-caption">Installing components into the printed enclosure</span>
          </div>
          <div className="image-card">
            <img src="images/week7/wiringincasing.jpg" alt="Wiring packed in casing" />
            <span className="image-caption">Tight internal wiring after final packing</span>
          </div>
        </div>
      </section>

      <section className="doc-section">
        <h3 className="section-title">Interface + Game MVP</h3>
        <div className="text-box">
          <p>
            Once I was confident that the device was fully functional, I built a simple menu-driven 
            interface and video game to demonstrate that the system was
            genuinely playable. The joystick is used for menu and game movement, and the button
            handles selection/restart logic. This clip shows the final result running on the MVP
            hardware.
          </p>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Menu + game mode flow</span>
          </div>
          <pre><code>{`enum class AppMode {
  Menu,
  Snake,
  Dodge,
};

void startSelectedGame() {
  if (menuIndex == 0) {
    appMode = AppMode::Snake;
    snakeGame.begin();
    return;
  }

  appMode = AppMode::Dodge;
  dodgeGame.begin();
}

void loop() {
  if (processButtonTaps()) {
    renderMenu();
    return;
  }

  if (appMode == AppMode::Snake) {
    snakeGame.update();
  } else {
    dodgeGame.update();
  }
}`}</code></pre>
        </div>

        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — Simple game movement logic</span>
          </div>
          <pre><code>{`void SimpleGame::updatePlayerLane() {
  Joystick::NormalizedReading norm = joystick_.readNormalized();
  float normalizedX = constrain(norm.x, -1.0f, 1.0f);

  if (normalizedX <= -kMoveThreshold) {
    playerLane_ = max(0, playerLane_ - 1);
    return;
  }

  if (normalizedX >= kMoveThreshold) {
    playerLane_ = min(kLaneCount - 1, playerLane_ + 1);
  }
}`}</code></pre>
        </div>

        <div className="download-row">
          <a href="/downloads/week7/SimpleGame.cpp" download className="download-btn">
            ⬇ DOWNLOAD SimpleGame.cpp
          </a>
        </div>

        <div className="video-container">
          <video controls playsInline>
            <source src="images/week7/demo.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Final MVP demo — interface and game running on hardware</span>
        </div>

        <div className="image-row">
          <div className="image-card">
            <img src="images/week7/final-rotated.jpg" alt="Final handheld front view" />
            <span className="image-caption">Front view of completed system</span>
          </div>
          <div className="image-card">
            <img src="images/week7/finalprofile.jpg" alt="Final handheld side profile" />
            <span className="image-caption">Final profile view of casing and component fit</span>
          </div>
        </div>
      </section>
    </div>
  )
}

export default Week7
