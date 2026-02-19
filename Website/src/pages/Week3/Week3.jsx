function Week3() {
  return (
    <div className="week-doc-content">
      {/* === SUMMARY === */}
      <section className="doc-section">
        <h3 className="section-title">Display Turntable — Kinetic Sculpture</h3>
        <div className="text-box">
          <p>
            This week's assignment was to design and build a kinetic sculpture. I decided to
            create a motorized display turntable driven by a gear train (a rotating platform that
            can showcase other cool stuff). This project was my first experience with mechanics,
            designing electrical circuits, and physical iteration upon an engineering design.
            Skills I used in this project was 3D CAD design, laser cutting, and hand assembly. 
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week3/Final.jpg" alt="Completed display turntable" />
            <span className="image-caption">The finished display turntable</span>
          </div>
        </div>
      </section>

      {/* === CAD PLAN === */}
      <section className="doc-section">
        <h3 className="section-title">Planning in CAD</h3>
        <div className="text-box">
          <p>
            I started by sketching out the gear train and platform layout in Fusion 360.
            The plan centered on a set of meshing spur gears that would transfer rotation
            from a small motor gear up to a larger turntable gear which would hold up the 
            raised platform. I was excited to start experimenting in the physical world
            so I rushed this process, not thinking to model how I'd house the moter and
            connect it to the gear train, which forced major iterations laters.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week3/CADplan.png" alt="CAD layout of the gear train" />
            <span className="image-caption">Gear train layout in Fusion 360</span>
          </div>
        </div>
      </section>

      {/* === BUILDING PROCESS === */}
      <section className="doc-section">
        <h3 className="section-title">Cutting &amp; Assembly</h3>
        <div className="text-box">
          <p>
            With the design locked in, I laser-cut all the parts out of cardboard and began
            putting them together. The build was an iterative process — test-fitting gears,
            adjusting shaft holes, and re-cutting pieces that were too tight or too loose.
            A big lesson I learned during this stage was that though cardboard was cheap,
            it was very structurally unreliable after being laser cut. Creating this out of
            wood would've both saved time from the failed cardboard parts I had to recut and
            would've left me with a more structurally sound final product. Each photo below 
            captures a step along the way as the turntable took shape.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week3/Building1.jpg" alt="Build progress — initial assembly" />
            <span className="image-caption">Initial assembly of the base and gears</span>
          </div>
          <div className="image-card">
            <img src="images/week3/Building2.jpg" alt="Build progress — gear fitting" />
            <span className="image-caption">Fitting the gear train together</span>
          </div>
          <div className="image-card">
            <img src="images/week3/Building3.jpg" alt="Build progress — platform mounting" />
            <span className="image-caption">Mounting the turntable platform</span>
          </div>
        </div>
      </section>

      {/* === WORKING GEARS === */}
      <section className="doc-section">
        <h3 className="section-title">Gears in Motion</h3>
        <div className="text-box">
          <p>
            As mentioned previously, I'd neglected to consider how I'd mount the motor
            in order to properly spin the gears without getting in the way of any movement
            and while keeping the turntable level. I ended up having to design a secondary
            plane below the gears to house the motor and battery.
            After some fine-tuning, I got the gear train to mesh properly and spin freely. This
            was essentially the finished product with as motion was being transferered
            smoothly from the motor shaft all the way up to the platform gear.
          </p>
        </div>
        <div className="video-container compact">
          <video controls playsInline>
            <source src="images/week3/WorkingGears.MOV" type="video/quicktime" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Working gear train test</span>
        </div>
      </section>

      {/* === CAD STICKMAN === */}
      <section className="doc-section">
        <h3 className="section-title">Test Passenger</h3>
        <div className="text-box">
          <p>
            To make sure the turntable could actually support and display an object, I
            modeled a quick stickman figure in Fusion 360 and placed it on the platform
            as a stand-in. It doubled as a fun proof-of-concept that the rotation was
            steady enough to show off a small, light model without wobble.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week3/CADStickman.png" alt="Stickman figure on turntable in CAD" />
            <span className="image-caption">Stickman test model on the turntable</span>
          </div>
        </div>
      </section>

      {/* === FINAL DEMO === */}
      <section className="doc-section">
        <h3 className="section-title">Final Demonstration</h3>
        <div className="text-box">
          <p>
            Here's the completed turntable in action. I wired a battery to the motor to provide
            a source of power that could neatly be stored within the mechanism.
          </p>
        </div>
        <div className="video-container">
          <video controls playsInline>
            <source src="images/week3/Final.MOV" type="video/quicktime" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">Display turntable — final demo</span>
        </div>
        <a
          href="/downloads/KineticSculpture.f3d"
          download
          className="download-btn"
        >
          ⬇ DOWNLOAD KINETIC SCULPTURE (.f3d)
        </a>
      </section>
    </div>
  )
}

export default Week3
