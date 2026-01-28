import { Link } from 'react-router-dom'

// 16-bit pixel art SVGs for each week's topic
const weekArt = {
  1: ( // Introduction - Welcome/Start screen
    <svg viewBox="0 0 64 64" className="pixel-art">
      <rect x="8" y="12" width="48" height="40" fill="#1a1a2e"/>
      <rect x="12" y="16" width="40" height="32" fill="#0f380f"/>
      <text x="32" y="28" textAnchor="middle" fill="#8bac0f" fontSize="6" fontFamily="monospace">PRESS</text>
      <text x="32" y="38" textAnchor="middle" fill="#9bbc0f" fontSize="8" fontFamily="monospace">START</text>
      <rect x="20" y="44" width="24" height="2" fill="#8bac0f">
        <animate attributeName="opacity" values="1;0;1" dur="1s" repeatCount="indefinite"/>
      </rect>
    </svg>
  ),
  2: ( // 2D Design - Pencil and ruler
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Pencil */}
      <rect x="12" y="40" width="32" height="6" fill="#f4d03f"/>
      <rect x="12" y="40" width="32" height="2" fill="#f9e076"/>
      <polygon points="44,40 44,46 52,43" fill="#ffb6c1"/>
      <polygon points="52,43 56,43 52,41 52,45" fill="#2c2c2c"/>
      <rect x="8" y="40" width="4" height="6" fill="#c0c0c0"/>
      {/* Ruler */}
      <rect x="14" y="18" width="36" height="10" fill="#3498db"/>
      <rect x="14" y="18" width="36" height="3" fill="#5dade2"/>
      {[0,1,2,3,4,5,6,7,8].map(i => (
        <rect key={i} x={18 + i*4} y="24" width="1" height={i%2===0 ? 4 : 2} fill="#fff"/>
      ))}
    </svg>
  ),
  3: ( // Fabrication - Hammer and saw
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Hammer */}
      <rect x="28" y="14" width="4" height="28" fill="#8b4513"/>
      <rect x="29" y="14" width="2" height="28" fill="#a0522d"/>
      <rect x="18" y="10" width="24" height="10" fill="#708090"/>
      <rect x="18" y="10" width="24" height="3" fill="#a8a8a8"/>
      {/* Saw teeth */}
      <rect x="10" y="44" width="44" height="4" fill="#c0c0c0"/>
      {[0,1,2,3,4,5,6,7,8,9,10].map(i => (
        <polygon key={i} points={`${12+i*4},48 ${14+i*4},52 ${16+i*4},48`} fill="#a0a0a0"/>
      ))}
      <rect x="46" y="40" width="8" height="12" fill="#8b4513"/>
    </svg>
  ),
  4: ( // Microcontroller - Circuit board
    <svg viewBox="0 0 64 64" className="pixel-art">
      <rect x="12" y="16" width="40" height="32" fill="#0d5c0d"/>
      <rect x="12" y="16" width="40" height="4" fill="#1a8c1a"/>
      {/* Chip */}
      <rect x="22" y="24" width="20" height="16" fill="#1c1c1c"/>
      <rect x="24" y="26" width="16" height="12" fill="#2d2d2d"/>
      {/* Pins */}
      {[0,1,2,3,4].map(i => (
        <g key={i}>
          <rect x={18} y={26+i*3} width="4" height="2" fill="#ffd700"/>
          <rect x={42} y={26+i*3} width="4" height="2" fill="#ffd700"/>
        </g>
      ))}
      {/* Traces */}
      <rect x="14" y="20" width="12" height="1" fill="#90ee90"/>
      <rect x="38" y="44" width="12" height="1" fill="#90ee90"/>
      {/* LED */}
      <circle cx="48" cy="20" r="2" fill="#ff0000">
        <animate attributeName="opacity" values="1;0.3;1" dur="0.5s" repeatCount="indefinite"/>
      </circle>
    </svg>
  ),
  5: ( // 3D Design - 3D Cube wireframe
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Back face */}
      <polygon points="24,16 44,16 44,36 24,36" fill="none" stroke="#00ffff" strokeWidth="2"/>
      {/* Front face */}
      <polygon points="16,24 36,24 36,44 16,44" fill="#1a1a2e" stroke="#ff00ff" strokeWidth="2"/>
      {/* Connecting lines */}
      <line x1="16" y1="24" x2="24" y2="16" stroke="#ffff00" strokeWidth="2"/>
      <line x1="36" y1="24" x2="44" y2="16" stroke="#ffff00" strokeWidth="2"/>
      <line x1="36" y1="44" x2="44" y2="36" stroke="#ffff00" strokeWidth="2"/>
      <line x1="16" y1="44" x2="24" y2="36" stroke="#ffff00" strokeWidth="2" strokeDasharray="2,2"/>
      {/* Rotation arrows */}
      <path d="M48,32 A8,8 0 1,1 48,31.9" fill="none" stroke="#00ff00" strokeWidth="1.5"/>
      <polygon points="50,28 48,32 52,32" fill="#00ff00"/>
    </svg>
  ),
  6: ( // Inputs - Sensors and buttons
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Push button */}
      <circle cx="20" cy="24" r="8" fill="#333"/>
      <circle cx="20" cy="24" r="6" fill="#ff4444"/>
      <circle cx="18" cy="22" r="2" fill="#ff8888"/>
      {/* Potentiometer */}
      <circle cx="44" cy="24" r="8" fill="#444"/>
      <circle cx="44" cy="24" r="6" fill="#666"/>
      <rect x="43" y="18" width="2" height="8" fill="#fff"/>
      {/* Light sensor */}
      <rect x="12" y="38" width="16" height="12" fill="#1a1a1a"/>
      <circle cx="20" cy="44" r="4" fill="#4a9eff"/>
      <circle cx="20" cy="44" r="2" fill="#8ec4ff">
        <animate attributeName="r" values="2;3;2" dur="1.5s" repeatCount="indefinite"/>
      </circle>
      {/* Touch sensor */}
      <rect x="36" y="38" width="16" height="12" fill="#2a4a6a"/>
      <rect x="38" y="40" width="12" height="8" fill="#3a6a9a"/>
      <text x="44" y="46" textAnchor="middle" fill="#fff" fontSize="4">TOUCH</text>
    </svg>
  ),
  7: ( // Outputs - LEDs and motor
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* RGB LEDs */}
      <circle cx="16" cy="20" r="5" fill="#ff0000">
        <animate attributeName="opacity" values="1;0.4;1" dur="0.8s" repeatCount="indefinite"/>
      </circle>
      <circle cx="32" cy="20" r="5" fill="#00ff00">
        <animate attributeName="opacity" values="0.4;1;0.4" dur="0.8s" repeatCount="indefinite"/>
      </circle>
      <circle cx="48" cy="20" r="5" fill="#0066ff">
        <animate attributeName="opacity" values="1;0.4;1" dur="1.2s" repeatCount="indefinite"/>
      </circle>
      {/* Motor */}
      <circle cx="32" cy="42" r="12" fill="#555"/>
      <circle cx="32" cy="42" r="10" fill="#777"/>
      <circle cx="32" cy="42" r="4" fill="#333"/>
      <rect x="30" y="30" width="4" height="12" fill="#444">
        <animateTransform attributeName="transform" type="rotate" from="0 32 42" to="360 32 42" dur="0.5s" repeatCount="indefinite"/>
      </rect>
      {/* Speaker icon */}
      <polygon points="8,38 8,46 12,46 18,50 18,34 12,38" fill="#8b4513"/>
    </svg>
  ),
  8: ( // CNC - Router/mill
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Machine frame */}
      <rect x="8" y="40" width="48" height="8" fill="#444"/>
      <rect x="8" y="40" width="48" height="2" fill="#666"/>
      {/* Rails */}
      <rect x="12" y="16" width="4" height="24" fill="#888"/>
      <rect x="48" y="16" width="4" height="24" fill="#888"/>
      <rect x="12" y="16" width="40" height="4" fill="#999"/>
      {/* Spindle */}
      <rect x="28" y="20" width="8" height="16" fill="#333"/>
      <rect x="30" y="36" width="4" height="6" fill="#c0c0c0"/>
      <polygon points="30,42 34,42 32,48" fill="#ffd700"/>
      {/* Material */}
      <rect x="20" y="48" width="24" height="6" fill="#deb887"/>
      {/* Cut path */}
      <path d="M24,51 L32,51 L32,52 L40,52" fill="none" stroke="#8b4513" strokeWidth="1"/>
    </svg>
  ),
  9: ( // Networking - WiFi and connections
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* WiFi symbol */}
      <path d="M32,44 L32,44" fill="none" stroke="#00ffff" strokeWidth="4" strokeLinecap="round"/>
      <path d="M24,38 Q32,30 40,38" fill="none" stroke="#00ffff" strokeWidth="2">
        <animate attributeName="opacity" values="0.3;1;0.3" dur="1s" repeatCount="indefinite"/>
      </path>
      <path d="M18,32 Q32,20 46,32" fill="none" stroke="#00ffff" strokeWidth="2">
        <animate attributeName="opacity" values="0.3;1;0.3" dur="1s" repeatCount="indefinite" begin="0.2s"/>
      </path>
      <path d="M12,26 Q32,10 52,26" fill="none" stroke="#00ffff" strokeWidth="2">
        <animate attributeName="opacity" values="0.3;1;0.3" dur="1s" repeatCount="indefinite" begin="0.4s"/>
      </path>
      {/* Data packets */}
      <rect x="10" y="50" width="4" height="4" fill="#ff00ff">
        <animate attributeName="x" values="10;50;10" dur="2s" repeatCount="indefinite"/>
      </rect>
      <rect x="50" y="54" width="4" height="4" fill="#ffff00">
        <animate attributeName="x" values="50;10;50" dur="2s" repeatCount="indefinite"/>
      </rect>
    </svg>
  ),
  10: ( // Machine Building - Robot
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Robot head */}
      <rect x="20" y="8" width="24" height="18" fill="#708090"/>
      <rect x="20" y="8" width="24" height="4" fill="#a0a0a0"/>
      {/* Eyes */}
      <rect x="24" y="14" width="6" height="6" fill="#000"/>
      <rect x="34" y="14" width="6" height="6" fill="#000"/>
      <rect x="26" y="16" width="2" height="2" fill="#ff0000">
        <animate attributeName="fill" values="#ff0000;#00ff00;#ff0000" dur="2s" repeatCount="indefinite"/>
      </rect>
      <rect x="36" y="16" width="2" height="2" fill="#ff0000">
        <animate attributeName="fill" values="#ff0000;#00ff00;#ff0000" dur="2s" repeatCount="indefinite"/>
      </rect>
      {/* Antenna */}
      <rect x="31" y="2" width="2" height="6" fill="#555"/>
      <circle cx="32" cy="2" r="2" fill="#ff0000"/>
      {/* Body */}
      <rect x="18" y="28" width="28" height="20" fill="#505050"/>
      <rect x="22" y="32" width="8" height="8" fill="#333"/>
      <rect x="34" y="32" width="8" height="8" fill="#333"/>
      {/* Gears */}
      <circle cx="26" cy="36" r="3" fill="#ffd700">
        <animateTransform attributeName="transform" type="rotate" from="0 26 36" to="360 26 36" dur="2s" repeatCount="indefinite"/>
      </circle>
      <circle cx="38" cy="36" r="3" fill="#ffd700">
        <animateTransform attributeName="transform" type="rotate" from="360 38 36" to="0 38 36" dur="2s" repeatCount="indefinite"/>
      </circle>
      {/* Arms */}
      <rect x="8" y="30" width="8" height="4" fill="#606060"/>
      <rect x="48" y="30" width="8" height="4" fill="#606060"/>
      {/* Legs */}
      <rect x="22" y="50" width="6" height="10" fill="#606060"/>
      <rect x="36" y="50" width="6" height="10" fill="#606060"/>
    </svg>
  ),
  11: ( // Final Project - Trophy
    <svg viewBox="0 0 64 64" className="pixel-art">
      {/* Trophy cup */}
      <path d="M20,16 L20,28 Q20,36 32,36 Q44,36 44,28 L44,16 Z" fill="#ffd700"/>
      <path d="M20,16 L20,22 Q20,26 32,26 Q44,26 44,22 L44,16 Z" fill="#ffec8b"/>
      {/* Handles */}
      <path d="M16,18 Q8,18 8,26 Q8,32 16,32" fill="none" stroke="#ffd700" strokeWidth="4"/>
      <path d="M48,18 Q56,18 56,26 Q56,32 48,32" fill="none" stroke="#ffd700" strokeWidth="4"/>
      {/* Base */}
      <rect x="28" y="36" width="8" height="8" fill="#daa520"/>
      <rect x="22" y="44" width="20" height="4" fill="#ffd700"/>
      <rect x="18" y="48" width="28" height="6" fill="#daa520"/>
      <rect x="18" y="48" width="28" height="2" fill="#ffd700"/>
      {/* Star */}
      <polygon points="32,20 34,26 40,26 35,30 37,36 32,32 27,36 29,30 24,26 30,26" fill="#fff">
        <animate attributeName="opacity" values="1;0.6;1" dur="1s" repeatCount="indefinite"/>
      </polygon>
      {/* Sparkles */}
      <circle cx="14" cy="12" r="1" fill="#fff">
        <animate attributeName="opacity" values="0;1;0" dur="1.5s" repeatCount="indefinite"/>
      </circle>
      <circle cx="50" cy="10" r="1" fill="#fff">
        <animate attributeName="opacity" values="0;1;0" dur="1.5s" repeatCount="indefinite" begin="0.5s"/>
      </circle>
      <circle cx="52" cy="40" r="1" fill="#fff">
        <animate attributeName="opacity" values="0;1;0" dur="1.5s" repeatCount="indefinite" begin="1s"/>
      </circle>
    </svg>
  ),
}

function Home({ weeks }) {
  return (
    <div className="home">
      <div className="arcade-header">
        <h1 className="main-title">PS70 PORTFOLIO</h1>
        <div className="pixel-divider"></div>
      </div>

      <div className="weeks-grid">
        {weeks.map((week) => (
          <Link
            key={week.id}
            to={`/week/${week.path}`}
            className="cartridge"
            data-color={week.color}
          >
            <div className="cartridge-top">
              <div className="cartridge-grip"></div>
            </div>
            <div className="cartridge-body">
              <div className="cartridge-label">
                <div className="label-header">
                  <span className="week-badge">WEEK {week.id}</span>
                </div>
                <div className="label-art">
                  {weekArt[week.id]}
                </div>
                <div className="label-title">
                  <span>{week.title.toUpperCase()}</span>
                </div>
              </div>
              <div className="cartridge-shine"></div>
            </div>
            <div className="cartridge-bottom">
              <div className="cartridge-pins"></div>
            </div>
          </Link>
        ))}
      </div>

      <div className="arcade-decoration">
        <div className="joystick"></div>
        <div className="buttons">
          <span className="btn-a"></span>
          <span className="btn-b"></span>
        </div>
      </div>
    </div>
  )
}

export default Home
