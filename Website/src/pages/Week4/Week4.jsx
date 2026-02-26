function Week4() {
  return (
    <div className="week-doc-content">
      {/* === INTRO === */}
      <section className="doc-section">
        <h3 className="section-title">Potentiometer-Controlled Turntable</h3>
        <div className="text-box">
          <p>
            This week's assignment focused on microcontroller programming so I decided to
            extend my display turntable from last week by designing a potentiometer
            circuit so I could control the rotation speed with a physical knob. This was my
            first time designing a circuit from scratch, wiring components on a breadboard,
            reading analog inputs with an ESP32, and using pulse-width modulation (PWM)
            to drive a motor at variable speeds. I'm very happy with the result as the 
            knob is quite responsive and acts exactly as I'd imagined. 
          </p>
        </div>
        <div className="video-container compact">
          <video controls playsInline>
            <source src="images/week4/final.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Final result — potentiometer-controlled turntable</span>
        </div>
      </section>

      {/* === CIRCUIT DESIGN === */}
      <section className="doc-section">
        <h3 className="section-title">Designing the Circuit</h3>
        <div className="text-box">
          <p>
            Below you can see the sketch of how my circuit ended up coming together. Many 
            thanks to Victor for his help during this process. The core idea was simple: 
            wire a potentiometer to an analog input pin on the
            ESP32, read its value, then map that reading to a voltage to send through the motor
            control unit.
            A large issue I encountered whilst trying to get the circuit to work was preventing
            noise from impairing the signal from the potentiometer. As a precaution, I added two
            capacitors along my ground line.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week4/circuitsketch.jpg" alt="Hand-drawn circuit schematic" />
            <span className="image-caption">Initial circuit sketch</span>
          </div>
        </div>
      </section>

      {/* === THE CODE === */}
      <section className="doc-section">
        <h3 className="section-title">Writing the Code</h3>
        <div className="text-box">
          <p>
            The Arduino sketch is fairly straightforward. In <code>setup()</code>, I configure the
            motor driver pins as outputs and initialize serial communication for debugging.
            Pin D7 drives the motor forward, while D8 is held LOW to set the
            direction. The potentiometer input is connected to analog pin D0.
          </p>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">Arduino — Setup</span>
          </div>
          <pre><code>{`// Motor 1
int A1A = D7;
int A1B = D8;
int potPin = D0;

void setup() {
  pinMode(A1A, OUTPUT);  
  pinMode(A1B, OUTPUT); 
  Serial.begin(9600);

  analogWrite(A1A, 0);
  digitalWrite(A1B, LOW);
}`}</code></pre>
        </div>
        <div className="text-box">
          <p>
            In the <code>loop()</code>, the potentiometer value is read every cycle using
            <code> analogRead()</code>. It's raw value (0–4095) is mapped to the range
            (255–0). The mapped value is then written
            to the motor pin with <code>analogWrite()</code>, which outputs a PWM signal
            that controls how fast the motor spins.
          </p>
        </div>
        <div className="code-block">
          <div className="code-header">
            <span className="code-lang">Arduino — Loop</span>
          </div>
          <pre><code>{`void loop() {
  int pot_value = analogRead(potPin);
  Serial.println(pot_value);
  int motor_speed = map(pot_value, 0, 4096, 255, 0);
  analogWrite(A1A, motor_speed);
  delay(1);
}`}</code></pre>
        </div>
        <div className="text-box">
          <p>
            The <code>Serial.println()</code> call was invaluable during development — it let
            me watch the raw potentiometer values stream into the Serial Monitor in real-time,
            making it easy to verify the knob was being read correctly before I'd
            the motor.
          </p>
        </div>
        <a
          href="downloads/TurntableCode.ino"
          download
          className="download-btn"
        >
          ⬇ DOWNLOAD ARDUINO CODE (.ino)
        </a>
      </section>

      {/* === DEBUGGING === */}
      <section className="doc-section">
        <h3 className="section-title">Building &amp; Debugging the Circuit</h3>
        <div className="text-box">
          <p>
            My first attempt had the potentiometer wired to pins adjactent to those for the motor.
            This was disrupting the serial reading from the potentiometer. After
            noticing this, I moved the motor pins away from the analog pins on the ESP32 which
            solved much of the noise.
            The photos below show the circuit during this assembly stage.
          </p>
        </div>
        <div className="image-row constrained">
          <div className="image-card">
            <img src="images/week4/circuitfull.jpg" alt="Full breadboard circuit" />
            <span className="image-caption">Full breadboard circuit with all components</span>
          </div>
          <div className="image-card">
            <img src="images/week4/circuitoverhead.jpg" alt="Overhead view of circuit" />
            <span className="image-caption">Overhead view — tracing the wiring</span>
          </div>
        </div>
      </section>

      {/* === FINAL RESULT === */}
      <section className="doc-section">
        <h3 className="section-title">Final Result</h3>
        <div className="text-box">
          <p>
            After long hours troubleshooting the issues with the circuit and with the code dialed in, 
            I finally connected everything to the
            turntable.
          </p>
        </div>
        <div className="image-row constrained">
          <div className="image-card">
            <img src="images/week4/finalfull.jpg" alt="Final turntable with potentiometer — full view" />
            <span className="image-caption">Complete setup — turntable with speed control</span>
          </div>
          <div className="image-card">
            <img src="images/week4/finalside.jpg" alt="Final turntable — side view" />
            <span className="image-caption">Side view of the final assembly</span>
          </div>
        </div>
      </section>
    </div>
  )
}

export default Week4
