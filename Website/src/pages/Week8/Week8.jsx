function Week8() {
  return (
    <div className="week-doc-content">
      {/* === PROJECT OVERVIEW === */}
      <section className="doc-section">
        <h3 className="section-title">Vacuum Forming &amp; CNC Sign</h3>
        <div className="text-box">
          <p>
            This week I duplicated a Stretch Armstrong-style figure using vacuum forming
            and a two-part silicone mold, then cast a final solid model. I also used the
            CNC mill in the shop to carve a simple sign from MDF for display.
          </p>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week8/base.jpg" alt="Original toy and base" />
            <span className="image-caption">Original toy used as the master</span>
          </div>
          <div className="image-card">
            <img src="images/week8/vacuum.jpg" alt="Vacuum forming setup" />
            <span className="image-caption">Vacuum forming the thin thermoformed shell</span>
          </div>
        </div>
      </section>

      {/* === SILICONE MOLD & CASTING === */}
      <section className="doc-section">
        <h3 className="section-title">Silicone Mold &amp; Casting</h3>
        <div className="text-box">
          <p>
            After vacuum forming a thin shell to capture the surface detail, I created a
            two-part silicone mold around the formed master. The silicone allowed us to
            pour casting resin to reproduce a durable copy of the toy. Key steps:
          </p>
          <ul>
            <li>Clean and secure the master on a flat base.</li>
            <li>Build a simple mold box and pour the first silicone half, embedding registration keys.</li>
            <li>After curing, flip and pour the second half to complete the mold.</li>
            <li>Demold, prep the cavity, then pour resin and allow full cure before trimming.</li>
          </ul>
        </div>
        <div className="image-row">
          <div className="image-card">
            <img src="images/week8/mold.jpg" alt="Silicone mold halves" />
            <span className="image-caption">Two-part silicone mold ready for casting</span>
          </div>
          <div className="image-card">
            <img src="images/week8/casting.jpg" alt="Casting the resin" />
            <span className="image-caption">Pouring resin into the silicone cavity</span>
          </div>
          <div className="image-card">
            <img src="images/week8/final_model.jpg" alt="Final cast model" />
            <span className="image-caption">Trimmed, sanded, and finished duplicate</span>
          </div>
        </div>
      </section>

      {/* === CNC SIGN === */}
      <section className="doc-section">
        <h3 className="section-title">CNC-Milled MDF Sign</h3>
        <div className="text-box">
          <p>
            I designed a simple nameplate and milled it from MDF on the shop CNC. The
            workflow included CAD for the outline and text, CAM toolpath generation,
            and a final carve using a straight-cut bit. After machining I sanded and
            applied finish paint to the sign face.
          </p>
        </div>
        <div className="image-row single">
          <div className="image-card">
            <img src="images/week8/final_sign.jpg" alt="Final CNC sign" />
            <span className="image-caption">MDF sign after finishing</span>
          </div>
        </div>
      </section>

      {/* Downloads section removed per request */}
    </div>
  )
}

export default Week8
