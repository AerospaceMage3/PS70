function About() {
  return (
    <div className="about-page">
      <div className="arcade-header">
        <h1 className="glitch" data-text="ABOUT ME">ABOUT ME</h1>
        <div className="pixel-divider"></div>
      </div>

      <div className="about-content">
        <div className="about-image-container">
          <div className="headshot-frame">
            <div className="headshot-placeholder">
              <div className="pixel-avatar">
                <span>PLAYER 1</span>
              </div>
              <div className="static-effect"></div>
            </div>
            <div className="frame-corner tl"></div>
            <div className="frame-corner tr"></div>
            <div className="frame-corner bl"></div>
            <div className="frame-corner br"></div>
          </div>
        </div>

        <div className="about-text">
          <div className="text-box">
            <p className="arcade-text">
              Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor 
              incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud 
              exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute 
              irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla 
              pariatur. Excepteur sint occaecat cupidatat non proident.
            </p>
            <p className="arcade-text">
              Sunt in culpa qui officia deserunt mollit anim id est laborum. Sed ut perspiciatis 
              unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam 
              rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae 
              vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur.
            </p>
          </div>
          <div className="stats-box">
            <h3>PLAYER STATS</h3>
            <div className="stat-bar">
              <span>CREATIVITY</span>
              <div className="bar"><div className="fill" style={{width: '85%'}}></div></div>
            </div>
            <div className="stat-bar">
              <span>FABRICATION</span>
              <div className="bar"><div className="fill" style={{width: '70%'}}></div></div>
            </div>
            <div className="stat-bar">
              <span>CODING</span>
              <div className="bar"><div className="fill" style={{width: '90%'}}></div></div>
            </div>
          </div>
        </div>
      </div>

      <div className="coin-slot">
        <span>● ● ●</span>
      </div>
    </div>
  )
}

export default About
