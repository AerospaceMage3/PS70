import { Link } from 'react-router-dom'
import Week1 from './Week1/Week1'
import Week2 from './Week2/Week2'

// Map week ids to their dedicated page components
const weekComponents = {
  1: Week1,
  2: Week2,
}

function WeekPage({ week }) {
  // Default content for weeks without a dedicated page
  const renderDefaultContent = () => (
    <div className="doc-placeholder">
      <h3>[ MISSION BRIEFING ]</h3>
      <p>
        This section will contain the documentation for Week {week.id}: {week.title}. 
        Future content about this week's project, process, and learnings will be added here.
      </p>
      <p>
        Add your images, code snippets, reflections, and other documentation to showcase 
        your work on this weekly assignment.
      </p>
      <div className="placeholder-image-area">
        <span>[ INSERT MEDIA HERE ]</span>
      </div>
    </div>
  )

  const PageComponent = weekComponents[week.id]

  return (
    <div className="week-page">
      <div className="arcade-header">
        <h1 className="glitch" data-text={`WEEK ${week.id}`}>WEEK {week.id}</h1>
        <h2 className="week-title">{week.title.toUpperCase()}</h2>
        <div className="pixel-divider"></div>
      </div>

      <div className="week-content">
        <div className="content-screen">
          <div className="documentation-area">
            {PageComponent ? <PageComponent /> : renderDefaultContent()}
          </div>
        </div>

        <div className="navigation-buttons">
          {week.id > 1 && (
            <Link to={`/week/${getPrevPath(week.id)}`} className="nav-btn prev">
              ◄ PREV LEVEL
            </Link>
          )}
          <Link to="/" className="nav-btn home">
            ▲ MAIN MENU
          </Link>
          {week.id < 11 && (
            <Link to={`/week/${getNextPath(week.id)}`} className="nav-btn next">
              NEXT LEVEL ►
            </Link>
          )}
        </div>
      </div>

      <div className="side-decorations">
        <div className="led-strip left"></div>
        <div className="led-strip right"></div>
      </div>
    </div>
  )
}

const weekPaths = [
  'intro', '2d-design', 'fabrication', 'microcontroller', '3d-design',
  'inputs', 'outputs', 'cnc', 'networking', 'machine', 'final-project'
]

function getPrevPath(currentId) {
  return weekPaths[currentId - 2]
}

function getNextPath(currentId) {
  return weekPaths[currentId]
}

export default WeekPage
