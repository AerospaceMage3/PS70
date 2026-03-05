import ModelViewer from '../../components/ModelViewer'

function Week5() {
  return (
    <div className="week-doc-content">
      {/* === D20 DESIGN === */}
      <section className="doc-section">
        <h3 className="section-title">3D Design</h3>
        <div className="text-box">
          <p>
            For this weeks assignemnt, I decided to model and 3D print a D20 die.
            Looking online, I find that dice sets are a bit overpriced so, seeing
            as I've been in need of some dice of my own for some of my personal
            hobbies, I think it would be a cool side project for me to 3D print
            my own set. Since the D20 is the most complex (and the most iconic)
            I thought it would be a good starting point. 
            Modeling it in Fusion 360 was a great exercise in designing more complex
            geometry. Getting the numbers on each face oriented
            correctly was a bit tedious however since each one had to be embossed into
            its triangular face with the right rotation so that it can read properly when
            the die sits on the opposite side. After a recommendation from a friend
            I rounded the corners of each face so that the die could roll smoothly.
          </p>
        </div>
        <div className="viewer-section">
          <ModelViewer
            mode="stl"
            stlUrl="models/D20.stl"
            color={0x3b82f6}
            caption="Interactive D20 model — drag to rotate, scroll to zoom"
          />
        </div>
      </section>

      {/* === 3D PRINTING === */}
      <section className="doc-section">
        <h3 className="section-title">3D Printing the D20</h3>
        <div className="text-box">
          <p>
            Once the CAD model was complete, I exported it and sliced it in PrusaSlicer
            with a 0.1 mm layer height for smooth faces. The print took about 40 minutes
            on a Prusa MK4S using PLA filament. The small size of the die and the 
            text on it meant that
            layer lines and surface finish really mattered. Fortunately, it came out 
            pretty nice on the first attempt.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week5/dice-photo.jpg" alt="3D-printed D20 die" />
            <span className="image-caption">The finished 3D-printed D20</span>
          </div>
        </div>
        <div className="video-container compact">
          <video controls playsInline>
            <source src="images/week5/dice-video.mp4" type="video/mp4" />
            Your browser does not support the video tag.
          </video>
          <span className="image-caption">D20 print showcase</span>
        </div>
        <div className="download-row">
          <a href="downloads/D20.f3d" download className="download-btn">
            ⬇ DOWNLOAD D20 CAD (.f3d)
          </a>
          <a href="models/D20.stl" download className="download-btn">
            ⬇ DOWNLOAD D20 (.stl)
          </a>
          <a
            href="downloads/Body6_0.4n_0.1mm_PLA_MK4S_40m.bgcode"
            download
            className="download-btn"
          >
            ⬇ DOWNLOAD GCODE (.bgcode)
          </a>
        </div>
      </section>

      {/* === 3D SCANNING === */}
      <section className="doc-section">
        <h3 className="section-title">3D Scanning — CS50 Rubber Duck</h3>
        <div className="text-box">
          <p>
            For the scanning portion, I used a Revopoint structured-light 3D scanner
            to digitize a CS50 rubber duck. I slowly rotated the duck on a turntable while the scanner
            captured hundreds of frames, which the software then aligned and merged
            into a single point cloud. After taking a few of these captures with the duck
            rotated in sufficiently different orientations, I could fuse them together for
            the complete scan.
          </p>
        </div>
        <div className="image-row single small">
          <div className="image-card">
            <img src="images/week5/scanning-photo.jpg" alt="Scanning the rubber duck with Revopoint" />
            <span className="image-caption">Scanning the CS50 duck with the Revopoint</span>
          </div>
        </div>
      </section>

      {/* === DUCK VIEWER === */}
      <section className="doc-section">
        <h3 className="section-title">Interactive Scan Viewer</h3>
        <div className="viewer-section">
          <ModelViewer
            mode="ply"
            plyUrl="models/daniel-duck_mesh.ply"
            caption="CS50 rubber duck — 3D scan mesh"
          />
        </div>
      </section>
    </div>
  )
}

export default Week5
