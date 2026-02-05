import { useState, useEffect } from 'react'

const bootSequence = [
  { text: 'INITIALIZING SYSTEM...', delay: 0 },
  { text: 'CHECKING MEMORY... OK', delay: 400 },
  { text: 'LOADING BIOS v1.0...', delay: 700 },
  { text: 'DETECTING HARDWARE...', delay: 1000 },
  { text: '  > CPU: 8-BIT PROCESSOR', delay: 1250 },
  { text: '  > RAM: 64KB DETECTED', delay: 1450 },
  { text: '  > VIDEO: CRT MODE', delay: 1650 },
  { text: '  > AUDIO: READY', delay: 1850 },
  { text: 'MOUNTING CARTRIDGE...', delay: 2150 },
  { text: 'VERIFYING DATA... OK', delay: 2500 },
  { text: '', delay: 2750 },
  { text: 'READY.', delay: 2950 },
]

function LoadingScreen({ onComplete }) {
  const [visibleLines, setVisibleLines] = useState([])
  const [phase, setPhase] = useState('boot') // 'boot' | 'fadeTerminal' | 'flash' | 'static' | 'logo' | 'done'
  const [glitchIntensity, setGlitchIntensity] = useState(0)

  useEffect(() => {
    // Show boot text lines sequentially
    bootSequence.forEach((line) => {
      setTimeout(() => {
        setVisibleLines(prev => [...prev, line.text])
      }, line.delay)
    })

    // Fade out terminal
    setTimeout(() => setPhase('fadeTerminal'), 3300)
    
    // Screen goes black, then flash
    setTimeout(() => setPhase('flash'), 4000)
    
    // Static/noise phase
    setTimeout(() => setPhase('static'), 4150)
    
    // Logo appears with glitch
    setTimeout(() => {
      setPhase('logo')
      // Glitch pulses
      setGlitchIntensity(1)
      setTimeout(() => setGlitchIntensity(0.3), 150)
      setTimeout(() => setGlitchIntensity(0.8), 300)
      setTimeout(() => setGlitchIntensity(0.1), 450)
      setTimeout(() => setGlitchIntensity(0.6), 550)
      setTimeout(() => setGlitchIntensity(0), 700)
    }, 4400)
    
    // Final fade out
    setTimeout(() => setPhase('done'), 6500)
    
    // Complete
    setTimeout(() => onComplete(), 7200)
  }, [onComplete])

  return (
    <div className={`loading-screen ${phase === 'done' ? 'fade-out' : ''}`}>
      <div className="crt-effect">
        {/* Boot sequence terminal */}
        <div className={`boot-sequence ${phase === 'fadeTerminal' ? 'fading' : ''} ${phase !== 'boot' && phase !== 'fadeTerminal' ? 'hidden' : ''}`}>
          {visibleLines.map((line, index) => (
            <div key={index} className="boot-line">
              {line}
              {index === visibleLines.length - 1 && line && phase === 'boot' && (
                <span className="cursor">█</span>
              )}
            </div>
          ))}
        </div>
        
        {/* White flash */}
        {phase === 'flash' && <div className="screen-flash" />}
        
        {/* Static noise */}
        {phase === 'static' && (
          <div className="static-noise">
            <div className="noise-overlay" />
          </div>
        )}
        
        {/* Logo with glitch effect */}
        {(phase === 'logo' || phase === 'done') && (
          <div className="boot-logo" style={{ '--glitch': glitchIntensity }}>
            <div className={`logo-container ${glitchIntensity > 0 ? 'glitching' : ''}`}>
              {/* Glitch layers */}
              <div className="glitch-layer red" aria-hidden="true">
                <span className="logo-ps">PS</span>
                <span className="logo-70">70</span>
              </div>
              <div className="glitch-layer cyan" aria-hidden="true">
                <span className="logo-ps">PS</span>
                <span className="logo-70">70</span>
              </div>
              {/* Main logo */}
              <div className="logo-text">
                <span className="logo-ps">PS</span>
                <span className="logo-70">70</span>
              </div>
            </div>
            <div className="logo-subtitle">DIGITAL FABRICATION</div>
            <div className="press-start">
              <span className="blink">PRESS START</span>
            </div>
          </div>
        )}
      </div>
      
      {/* Scanline overlay for CRT effect */}
      <div className="loading-scanlines"></div>
      
      {/* Screen flicker effect */}
      <div className="screen-flicker"></div>
    </div>
  )
}

export default LoadingScreen
