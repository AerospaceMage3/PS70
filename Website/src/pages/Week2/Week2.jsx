function Week2() {
  return (
    <div className="week-doc-content">
      {/* === LASER-CUT BOX SECTION === */}
      <section className="doc-section">
        <h3 className="section-title">Laser-Cut Box</h3>
        <div className="text-box">
          <p>
            This week, I learned CAD modeling in Fusion 360, how to use machine tools including
            the band-saw and drill press, and how to lazer cut objects. For the assignment, 
            I designed and laser-cut a cardboard  box using Fusion 360. I started by modeling 
            each of the box panels with finger joints, carefully accounting for the kerf of the
            laser cutter so that the pieces would press-fit together with minimal adhesive. 
            After exporting the project as a DXF file, I brought them into the laser cutter 
            software and cut them out of cardboard. As a touch of personality on my box, I
            rastor-engraved the California flag onto the front panel.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week2/box-photo.png" alt="Laser-cut box assembled" />
            <span className="image-caption">Laser-cut box as glue drys</span>
          </div>
          <div className="image-card">
            <img src="images/week2/box-cad.png" alt="Box CAD design" />
            <span className="image-caption">Box CAD projection</span>
          </div>
        </div>
      </section>

      {/* === PEN SECTION === */}
      <section className="doc-section">
        <h3 className="section-title">Pen Models</h3>
        <div className="text-box">
          <p>
            I also chose to model a desk pen in as an exercise. The body 
            doing this I found a real love for the Revolve and Filet tools, they
            are really capable of easily producing some elegant shapes.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week2/pen-cad-1.png" alt="Pen CAD model view 1" />
            <span className="image-caption">Pen model — side view</span>
          </div>
          <div className="image-card">
            <img src="images/week2/pen-cad-2.png" alt="Pen CAD model view 2" />
            <span className="image-caption">Pen model — full view</span>
          </div>
        </div>
        <a
          href="/downloads/Pen.f3d"
          download
          className="download-btn"
        >
          ⬇ DOWNLOAD PEN (.f3d)
        </a>
      </section>

      {/* === BUTTON SECTION === */}
      <section className="doc-section">
        <h3 className="section-title">Button Model</h3>
        <div className="text-box">
          <p>
            Finally, I modeled a push-button, inspired by a real button I
            have in my dorm room that my roommates and I get some fun out of. Using 
            my newfound love for the revolve tool, I realized that, since the button 
            is perfectly symmetric about its center axis, I could simply model half of
            its cross section in order to create the final shape. I added a couple
            aestheic final touches to finish of the replication, including its nice
            yellow paint job and text across the top.

          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week2/button-cad-1.png" alt="Button CAD model view 1" />
            <span className="image-caption">Button model — top view</span>
          </div>
          <div className="image-card">
            <img src="images/week2/button-cad-2.png" alt="Button CAD model view 2" />
            <span className="image-caption">Button model — full view</span>
          </div>
        </div>
        <a
          href="/downloads/Button.f3d"
          download
          className="download-btn"
        >
          ⬇ DOWNLOAD BUTTON (.f3d)
        </a>
      </section>
    </div>
  )
}

export default Week2
