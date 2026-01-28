import { Link } from 'react-router-dom'

function WeekPage({ week }) {
  // Week 1 Introduction - simple editable list
  const renderIntroContent = () => (
    <div className="simple-content">
      <h3>Final Project Ideas</h3>
      <ul>
        <li>Idea 1 - Description here</li>
        <li>Idea 2 - Description here</li>
        <li>Idea 3 - Description here</li>
        <li>Idea 4 - Description here</li>
        <li>Idea 5 - Description here</li>
      </ul>
    </div>
  )

  // Default content for other weeks
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
            {week.id === 1 ? renderIntroContent() : renderDefaultContent()}
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
