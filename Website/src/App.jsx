import { useState, useEffect } from 'react'
import { Routes, Route, useLocation } from 'react-router-dom'
import Navbar from './components/Navbar'
import LoadingScreen from './components/LoadingScreen'
import Home from './pages/Home'
import About from './pages/About'
import WeekPage from './pages/WeekPage'

const weeks = [
  { id: 1, title: 'Introduction', path: 'intro', color: 'white' },
  { id: 2, title: '2D Design', path: '2d-design', color: 'cyan' },
  { id: 3, title: 'Fabrication', path: 'fabrication', color: 'brown' },
  { id: 4, title: 'Microcontroller', path: 'microcontroller', color: 'green' },
  { id: 5, title: '3D Design', path: '3d-design', color: 'purple' },
  { id: 6, title: 'Inputs', path: 'inputs', color: 'teal' },
  { id: 7, title: 'Outputs', path: 'outputs', color: 'red' },
  { id: 8, title: 'CNC', path: 'cnc', color: 'silver' },
  { id: 9, title: 'Networking', path: 'networking', color: 'blue' },
  { id: 10, title: 'Machine Building', path: 'machine', color: 'orange' },
  { id: 11, title: 'Final Project', path: 'final-project', color: 'gold' },
]

function App() {
  const [isLoading, setIsLoading] = useState(true)
  const location = useLocation()

  // Show boot intro on initial visit (once per session)
  useEffect(() => {
    const hasSeenIntro = sessionStorage.getItem('hasSeenIntro')
    if (!hasSeenIntro && location.pathname === '/') {
      setIsLoading(true)
    } else {
      setIsLoading(false)
    }
  }, [location.pathname])

  const handleLoadingComplete = () => {
    sessionStorage.setItem('hasSeenIntro', 'true')
    setIsLoading(false)
  }

  if (isLoading) {
    return <LoadingScreen onComplete={handleLoadingComplete} />
  }

  return (
    <div className="app">
      <Navbar weeks={weeks} />
      <main className="main-content">
        <Routes>
          <Route path="/" element={<Home weeks={weeks} />} />
          <Route path="/about" element={<About />} />
          {weeks.map((week) => (
            <Route
              key={week.id}
              path={`/week/${week.path}`}
              element={<WeekPage week={week} />}
            />
          ))}
        </Routes>
      </main>
      <footer className="arcade-footer">
        <p>INSERT COIN TO CONTINUE</p>
        <div className="scanlines"></div>
      </footer>
    </div>
  )
}

export default App
