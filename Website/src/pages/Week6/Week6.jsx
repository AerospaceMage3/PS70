function Week6() {
  return (
    <div className="week-doc-content">
      {/* === CAPACITIVE WATER SENSOR === */}
      <section className="doc-section">
        <h3 className="section-title">Capacitive Water-Level Sensor</h3>
        <div className="text-box">
          <p>
            For this week's group project, my teammates and I built a device that
            tracks the volume of water in a glass using capacitive sensing. Two
            strips of copper tape run vertically along opposite sides of the glass
            and act as the plates of a capacitor. As the water level rises, the
            dielectric between the plates changes — air is replaced by water, which
            has a much higher permittivity — causing the measured capacitance to
            increase proportionally with the water height.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week6/container.jpg" alt="Glass with copper tape electrodes" />
            <span className="image-caption">Glass fitted with copper-tape capacitive electrodes</span>
          </div>
        </div>
      </section>

      {/* === CIRCUIT & SETUP === */}
      <section className="doc-section">
        <h3 className="section-title">Circuit &amp; Setup</h3>
        <div className="text-box">
          <p>
            One electrode connects to a digital output pin (the transmitter) and the
            other to an analog input pin (the receiver). The ESP32 steps the
            transmitter pin high, then immediately reads the voltage that couples
            across to the receiver. The difference between the high and low readings
            is proportional to the capacitance between the two strips. We average
            100 samples per reading to reduce noise.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week6/containercircuit.jpg" alt="Breadboard circuit for capacitive sensor" />
            <span className="image-caption">Breadboard wiring for the capacitive sensor</span>
          </div>
          <div className="image-card">
            <img src="images/week6/containersetup.jpg" alt="Full test setup with glass and ESP32" />
            <span className="image-caption">Full test setup — glass, electrodes, and ESP32</span>
          </div>
        </div>
      </section>

      {/* === CODE === */}
      <section className="doc-section">
        <h3 className="section-title">The Code</h3>
        <div className="text-box">
          <p>
            The core of the firmware is the <code>tx_rx()</code> function. It toggles
            the transmitter pin, reads the analog response, and accumulates the
            difference over 100 samples. The summed value scales linearly with the
            water height.
          </p>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — tx_rx sensing function</span>
          </div>
          <pre><code>{`long tx_rx() {
  int read_high;
  int read_low;
  int diff;
  long int sum;
  int N_samples = 100;

  sum = 0;

  for (int i = 0; i < N_samples; i++){
    digitalWrite(tx_pin, HIGH);
    read_high = analogRead(analog_pin);
    delayMicroseconds(100);
    digitalWrite(tx_pin, LOW);
    read_low = analogRead(analog_pin);
    diff = read_high - read_low;
    sum += diff;
  }
  return sum;
}`}</code></pre>
        </div>
        <div className="text-box">
          <p>
            After we collected a set of data readings from the sensors, we could
            model a linear regression that converts the raw sum into a height value.
            In the main loop we also light an LED when the reading crosses a
            threshold — a quick visual indicator that the glass is above a certain
            fill level.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week6/waterlevel-graph.png" alt="Graph of water level over units of capacity" />
            <span className="image-caption">Linear regression — water level vs. capacitance readings</span>
          </div>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">C++ — calibration &amp; main loop</span>
          </div>
          <pre><code>{`long get_height(float signal) {
  return round((0.003354 * signal) - 356.59);
}

void loop() {
  result = tx_rx();
  Serial.println(result);

  if (result > 380000) {
    digitalWrite(led_pin, HIGH);
  } else {
    digitalWrite(led_pin, LOW);
  }
}`}</code></pre>
        </div>
        <a
          href="downloads/CapacitiveTouch.cpp"
          download
          className="download-btn"
        >
          ⬇ DOWNLOAD SOURCE CODE (.cpp)
        </a>
      </section>

      {/* === DEMO VIDEO === */}
      <section className="doc-section">
        <h3 className="section-title">Water Sensor Demo</h3>
        <div className="video-container compact">
          <video controls playsInline>
            <source src="images/week6/containerdemo.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Capacitive water-level sensor in action</span>
        </div>
      </section>

      {/* === LIGHT SENSOR === */}
      <section className="doc-section">
        <h3 className="section-title">Light-Activated LED Circuit</h3>
        <div className="text-box">
          <p>
            For the individual portion of this week, I built a circuit that uses a
            photoresistor to detect ambient light levels. When the room gets
            dark (or when I cover the sensor with my hand) the resistance of the
            LDR rises, pulling the analog reading below a threshold. The ESP32
            responds by turning on an LED, effectively creating an automatic
            nightlight. 
          </p>
        </div>
        <div className="image-row constrained">
          <div className="image-card">
            <img src="images/week6/lightsensorcircuit.jpg" alt="Light sensor breadboard circuit" />
            <span className="image-caption">Light sensor circuit on the breadboard</span>
          </div>
          <div className="image-card">
            <img src="images/week6/lightsensor2.jpg" alt="Light sensor close-up" />
            <span className="image-caption">Close-up of the photoresistor</span>
          </div>
        </div>
        <div className="video-container compact">
          <video controls playsInline>
            <source src="images/week6/lightsensor.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Light sensor demo — LED turns on when sensor is covered</span>
        </div>
      </section>
    </div>
  )
}

export default Week6
