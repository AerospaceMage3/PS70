import { useState } from 'react'
import { Link, useNavigate } from 'react-router-dom'

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
    </nav>
  )
}

export default Navbar
