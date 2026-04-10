function Week8() {
  return (
    <div className="week-doc-content">
      {/* === PROJECT OVERVIEW === */}
      <section className="doc-section">
        <h3 className="section-title">ESP-NOW RC Car</h3>
        <div className="text-box">
          <p>
            This week my team and I built a rear-wheel-drive RC car controlled over
            ESP-NOW. One ESP32 board acts as the handheld controller, sending joystick
            direction states wirelessly, while a second ESP32 on the car receives those
            packets and drives both the motor and steering. The drivetrain is powered
            through the rear wheels, and steering is handled by a front servo mechanism.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week8/final.jpg" alt="Final RC car" />
            <span className="image-caption">Completed ESP-NOW RC car</span>
          </div>
        </div>
      </section>

      {/* === MECHANICAL DESIGN === */}
      <section className="doc-section">
        <h3 className="section-title">Drive &amp; Steering Mechanism</h3>
        <div className="text-box">
          <p>
            Mechanically, the car uses a rear-wheel drive motor stage for propulsion and
            a servo linkage for steering. The motor gives forward/reverse motion on the
            back axle, while the servo angle controls the front wheel direction. This
            separation made the control logic clean: the Y joystick axis maps to throttle
            and the X axis maps to steering.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week8/chasis.jpg" alt="RC car chassis" />
            <span className="image-caption">Chassis layout during assembly</span>
          </div>
          <div className="image-card">
            <img src="images/week8/motor.jpg" alt="Rear drive motor setup" />
            <span className="image-caption">Rear motor drive assembly</span>
          </div>
          <div className="image-card">
            <img src="images/week8/steeringmech.jpg" alt="Servo steering mechanism" />
            <span className="image-caption">Servo-based steering mechanism</span>
          </div>
        </div>
      </section>

      {/* === ELECTRONICS & COMMS === */}
      <section className="doc-section">
        <h3 className="section-title">Electronics &amp; ESP-NOW Communication</h3>
        <div className="text-box">
          <p>
            We used ESP-NOW for low-latency peer-to-peer control messages. The controller
            reads joystick values, converts each axis into discrete states (-1, 0, 1),
            and broadcasts a small data struct. On the receiver side, the car listens for
            incoming packets, checks packet size, then updates steering and drive outputs.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week8/circuit.jpg" alt="RC car electronics circuit" />
            <span className="image-caption">Controller/receiver wiring and power electronics</span>
          </div>
        </div>
      </section>

      {/* === CONTROLLER CODE === */}
      <section className="doc-section">
        <h3 className="section-title">Controller Code Snippet</h3>
        <div className="text-box">
          <p>
            The controller packages joystick direction states into a lightweight struct
            and sends it over ESP-NOW every loop cycle.
          </p>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — controller.cpp</span>
          </div>
          <pre><code>{`typedef struct struct_message {
  int x;
  int y;
} struct_message;

struct_message myData;

void loop() {
  int rawX = joystick.readX();
  int rawY = joystick.readY();

  myData.x = joystick.axisStateX(rawX);
  myData.y = joystick.axisStateY(rawY);

  esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  delay(500);
}`}</code></pre>
        </div>
      </section>

      {/* === RECEIVER CODE === */}
      <section className="doc-section">
        <h3 className="section-title">Receiver Code Snippet</h3>
        <div className="text-box">
          <p>
            The receiver unpacks the same struct, then maps X to servo steering and Y
            to motor direction for real-time control of the car.
          </p>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — main.cpp (receiver)</span>
          </div>
          <pre><code>{`void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len != sizeof(receivedData)) return;
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  newDataReceived = true;
}

void loop() {
  if (newDataReceived) {
    newDataReceived = false;
    steeringServo.updateFromDiscreteInput(receivedData.x);
    driveMotor.updateFromDiscreteInput(receivedData.y);
  }
  delay(50);
}`}</code></pre>
        </div>
        <div className="download-row">
          <a href="downloads/RCController.cpp" download className="download-btn">
            ⬇ DOWNLOAD CONTROLLER CODE
          </a>
          <a href="downloads/RCReceiver.cpp" download className="download-btn">
            ⬇ DOWNLOAD RECEIVER CODE
          </a>
        </div>
      </section>

      {/* === DEMO === */}
      <section className="doc-section">
        <h3 className="section-title">Demo</h3>
        <div className="video-container compact">
          <video
            controls
            playsInline
            muted
            defaultMuted
            onVolumeChange={(event) => {
              event.currentTarget.muted = true
              event.currentTarget.volume = 0
            }}
          >
            <source src="images/week8/demo.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">ESP-NOW RC car driving demo</span>
        </div>
      </section>
    </div>
  )
}

export default Week8
