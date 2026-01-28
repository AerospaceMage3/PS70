// 16-bit icons for about page
const icons = {
  house: (
    <svg viewBox="0 0 32 32" className="bullet-icon">
      {/* Red brick building base */}
      <rect x="6" y="22" width="20" height="8" fill="#8b2500"/>
      <rect x="8" y="20" width="16" height="4" fill="#a52a2a"/>
      {/* Windows on brick */}
      <rect x="9" y="24" width="3" height="4" fill="#2a2a2a"/>
      <rect x="14" y="24" width="4" height="4" fill="#2a2a2a"/>
      <rect x="20" y="24" width="3" height="4" fill="#2a2a2a"/>
      {/* White tower/cupola base */}
      <rect x="11" y="12" width="10" height="8" fill="#f5f5f5"/>
      <rect x="11" y="12" width="10" height="2" fill="#e8e8e8"/>
      {/* Cupola windows */}
      <rect x="13" y="15" width="2" height="3" fill="#4a4a4a"/>
      <rect x="17" y="15" width="2" height="3" fill="#4a4a4a"/>
      {/* White upper cupola section */}
      <rect x="12" y="8" width="8" height="4" fill="#ffffff"/>
      <rect x="14" y="9" width="1" height="2" fill="#4a4a4a"/>
      <rect x="17" y="9" width="1" height="2" fill="#4a4a4a"/>
      {/* Green/teal dome */}
      <ellipse cx="16" cy="7" rx="5" ry="3" fill="#2e8b7b"/>
      <ellipse cx="16" cy="6" rx="4" ry="2" fill="#3cb39a"/>
      {/* Gold spire on top */}
      <rect x="15" y="1" width="2" height="4" fill="#ffd700"/>
      <polygon points="16,0 15,2 17,2" fill="#ffd700"/>
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
      {/* Bell of saxophone */}
      <ellipse cx="10" cy="26" rx="6" ry="4" fill="#daa520"/>
      <ellipse cx="10" cy="26" rx="4" ry="2.5" fill="#1a1a1a"/>
      {/* Body curve */}
      <path d="M14,24 Q18,20 18,14 L18,8 Q18,6 20,6 L24,6" fill="none" stroke="#daa520" strokeWidth="4"/>
      <path d="M14,24 Q18,20 18,14 L18,8 Q18,6 20,6 L24,6" fill="none" stroke="#b8860b" strokeWidth="2"/>
      {/* Mouthpiece */}
      <rect x="24" y="4" width="4" height="4" fill="#2a2a2a"/>
      <rect x="27" y="5" width="3" height="2" fill="#1a1a1a"/>
      {/* Keys */}
      <circle cx="17" cy="12" r="1.5" fill="#c0c0c0"/>
      <circle cx="16" cy="16" r="1.5" fill="#c0c0c0"/>
      <circle cx="14" cy="20" r="1.5" fill="#c0c0c0"/>
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
      {/* Globe/Earth for travel */}
      <circle cx="16" cy="16" r="12" fill="#4169e1"/>
      <ellipse cx="16" cy="16" rx="12" ry="5" fill="none" stroke="#2e5090" strokeWidth="1"/>
      <ellipse cx="16" cy="16" rx="5" ry="12" fill="none" stroke="#2e5090" strokeWidth="1"/>
      <line x1="4" y1="16" x2="28" y2="16" stroke="#2e5090" strokeWidth="1"/>
      {/* Continents simplified */}
      <ellipse cx="12" cy="12" rx="4" ry="3" fill="#228b22"/>
      <ellipse cx="20" cy="14" rx="3" ry="4" fill="#228b22"/>
      <ellipse cx="14" cy="20" rx="3" ry="2" fill="#228b22"/>
      {/* Airplane */}
      <polygon points="24,6 26,8 30,8 26,10 24,10 22,12 20,10 22,8" fill="#fff"/>
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
              <img src="/images/headshot.jpg" alt="Daniel Spadafora" className="headshot-image" />
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
