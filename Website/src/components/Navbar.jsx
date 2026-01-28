import { useState } from 'react'
import { Link, useNavigate } from 'react-router-dom'

// Track & Field style 16-bit sprinter - tall and lean, leaning forward
const Sprinter = () => (
  <svg viewBox="0 0 14 20" className="sprinter-sprite">
    {/* Hair */}
    <rect x="7" y="0" width="4" height="2" fill="#3d2817"/>
    <rect x="6" y="1" width="1" height="2" fill="#3d2817"/>
    {/* Head */}
    <rect x="7" y="2" width="4" height="4" fill="#f5d0a9"/>
    <rect x="6" y="3" width="1" height="2" fill="#f5d0a9"/>
    {/* Eye */}
    <rect x="9" y="3" width="1" height="1" fill="#222"/>
    {/* Neck */}
    <rect x="7" y="6" width="2" height="1" fill="#e8bc8a"/>
    {/* Torso - orange tank, shifted forward */}
    <rect x="5" y="7" width="4" height="5" fill="#e85820"/>
    <rect x="6" y="7" width="2" height="1" fill="#f07830"/>
    {/* Back arm */}
    <rect className="arm-back" x="3" y="8" width="3" height="2" fill="#e8bc8a"/>
    {/* Front arm */}
    <rect className="arm-front" x="8" y="7" width="4" height="2" fill="#f5d0a9"/>
    {/* Shorts - blue */}
    <rect x="4" y="12" width="4" height="2" fill="#2858a8"/>
    {/* Back leg */}
    <rect className="leg-back" x="2" y="14" width="2" height="3" fill="#e8bc8a"/>
    <rect className="foot-back" x="0" y="17" width="3" height="2" fill="#f5d0a9"/>
    {/* Front leg */}
    <rect className="leg-front" x="6" y="14" width="2" height="4" fill="#f5d0a9"/>
    <rect className="foot-front" x="7" y="18" width="3" height="2" fill="#fff"/>
  </svg>
)

function Navbar({ weeks }) {
  const [dropdownOpen, setDropdownOpen] = useState(false)
  const navigate = useNavigate()

  const handleWeekSelect = (path) => {
    navigate(`/week/${path}`)
    setDropdownOpen(false)
  }

  return (
    <nav className="navbar">
      <div className="navbar-container">
        <Link to="/" className="navbar-brand">
          <span className="brand-text">DANIEL SPADAFORA</span>
          <span className="brand-blink">_</span>
        </Link>

        <div className="navbar-links">
          <div className="dropdown">
            <button 
              className="dropdown-toggle arcade-btn"
              onClick={() => setDropdownOpen(!dropdownOpen)}
            >
              WEEKLY REPORTS ▼
            </button>
            {dropdownOpen && (
              <div className="dropdown-menu">
                {weeks.map((week) => (
                  <button
                    key={week.id}
                    className="dropdown-item"
                    onClick={() => handleWeekSelect(week.path)}
                  >
                    WEEK {week.id}: {week.title.toUpperCase()}
                  </button>
                ))}
              </div>
            )}
          </div>

          <Link to="/about" className="nav-link arcade-btn">
            ABOUT ME
          </Link>
        </div>
      </div>
      <div className="navbar-glow"></div>
      
      {/* Running sprinter */}
      <div className="sprinter-container">
        <Sprinter />
      </div>
    </nav>
  )
}

export default Navbar
