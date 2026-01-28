// 16-bit icons for about page
const icons = {
  house: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <polygon points="16,4 4,14 4,28 28,28 28,14" fill="#8b4513"/>
      <polygon points="16,4 4,14 28,14" fill="#d2691e"/>
      <rect x="12" y="18" width="8" height="10" fill="#4a3728"/>
      <rect x="7" y="16" width="5" height="5" fill="#87ceeb"/>
      <rect x="20" y="16" width="5" height="5" fill="#87ceeb"/>
    </svg>
  ),
  computer: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <rect x="4" y="6" width="24" height="16" fill="#333" rx="2"/>
      <rect x="6" y="8" width="20" height="12" fill="#0f380f"/>
      <text x="16" y="16" textAnchor="middle" fill="#9bbc0f" fontSize="6" fontFamily="monospace">&gt;_</text>
      <rect x="10" y="24" width="12" height="2" fill="#555"/>
      <rect x="8" y="26" width="16" height="2" fill="#444"/>
    </svg>
  ),
  location: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <path d="M16,2 C10,2 6,7 6,12 C6,20 16,30 16,30 C16,30 26,20 26,12 C26,7 22,2 16,2" fill="#ff4444"/>
      <circle cx="16" cy="12" r="4" fill="#fff"/>
      <path d="M16,2 C10,2 6,7 6,12 C6,14 7,16 8,18" fill="#ff6666" opacity="0.5"/>
    </svg>
  ),
  saxophone: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <path d="M8,28 Q4,24 6,20 L12,8 Q14,4 18,4 L22,4 Q24,4 24,6 L24,8 L20,8 L14,20 Q12,24 14,28" fill="#ffd700"/>
      <circle cx="10" cy="22" r="1.5" fill="#333"/>
      <circle cx="12" cy="18" r="1.5" fill="#333"/>
      <circle cx="14" cy="14" r="1.5" fill="#333"/>
      <ellipse cx="22" cy="6" rx="3" ry="2" fill="#333"/>
    </svg>
  ),
  gamepad: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <rect x="4" y="10" width="24" height="14" fill="#444" rx="4"/>
      <rect x="8" y="14" width="2" height="6" fill="#222"/>
      <rect x="6" y="16" width="6" height="2" fill="#222"/>
      <circle cx="22" cy="15" r="2" fill="#ff4444"/>
      <circle cx="26" cy="17" r="2" fill="#4444ff"/>
      <rect x="14" y="20" width="4" height="1" fill="#222"/>
    </svg>
  ),
  plane: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <polygon points="16,2 12,10 4,12 12,14 12,22 16,18 20,22 20,14 28,12 20,10" fill="#87ceeb"/>
      <polygon points="16,2 12,10 16,8 20,10" fill="#add8e6"/>
      <rect x="14" y="22" width="4" height="6" fill="#87ceeb"/>
      <polygon points="10,28 16,24 22,28" fill="#87ceeb"/>
    </svg>
  ),
  movie: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      <rect x="4" y="6" width="24" height="20" fill="#333"/>
      <rect x="6" y="8" width="20" height="14" fill="#1a1a2e"/>
      {[0,1,2,3,4].map(i => (
        <g key={i}>
          <rect x="4" y={8 + i*4} width="3" height="2" fill="#ffd700"/>
          <rect x="25" y={8 + i*4} width="3" height="2" fill="#ffd700"/>
        </g>
      ))}
      <polygon points="14,11 14,19 20,15" fill="#fff"/>
    </svg>
  ),
}

function About() {
  return (
    <div className="about-page">
      <div className="arcade-header">
        <h1 className="glitch" data-text="ABOUT ME">ABOUT ME</h1>
        <div className="pixel-divider"></div>
      </div>

      <div className="about-content">
        <div className="about-left-column">
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

          <div className="info-box">
            <h3>PLAYER INFO</h3>
            <ul className="pixel-list">
              <li>
                {icons.house}
                <span>Eliot House</span>
              </li>
              <li>
                {icons.computer}
                <span>Computer Science</span>
              </li>
              <li>
                {icons.location}
                <span>From Santa Cruz, CA</span>
              </li>
            </ul>
          </div>

          <div className="info-box interests-box">
            <h3>INTERESTS</h3>
            <ul className="pixel-list">
              <li>
                {icons.saxophone}
                <span>Jazz Saxophone</span>
              </li>
              <li>
                {icons.gamepad}
                <span>Video and Board Games</span>
              </li>
              <li>
                {icons.plane}
                <span>Traveling</span>
              </li>
              <li>
                {icons.movie}
                <span>Movies and TV</span>
              </li>
            </ul>
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
        </div>
      </div>

      <div className="coin-slot">
        <span>● ● ●</span>
      </div>
    </div>
  )
}

export default About
