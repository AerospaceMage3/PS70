import { useRef, useEffect, useState } from 'react'
import * as THREE from 'three'
import { OrbitControls } from 'three/addons/controls/OrbitControls.js'
import { PLYLoader } from 'three/addons/loaders/PLYLoader.js'
import { STLLoader } from 'three/addons/loaders/STLLoader.js'

/**
 * Interactive 3D viewer component.
 *
 * Props:
 *   mode        – "stl" | "ply"
 *   stlUrl      – path to an STL file (when mode === "stl")
 *   plyUrl      – path to a binary PLY file (when mode === "ply")
 *   color       – hex colour for the STL model (default 0x8b5cf6)
 *   caption     – optional label below the canvas
 */
function ModelViewer({ mode = 'stl', stlUrl, plyUrl, color = 0x8b5cf6, caption }) {
  const mountRef = useRef(null)
  const [loading, setLoading] = useState(true)
  const [error, setError] = useState(null)

  useEffect(() => {
    const container = mountRef.current
    if (!container) return

    /* ── renderer ─────────────────────────────── */
    const width = container.clientWidth
    const height = 420
    const renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true })
    renderer.setSize(width, height)
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2))
    renderer.setClearColor(0x000000, 0)
    container.appendChild(renderer.domElement)

    /* ── scene & camera ───────────────────────── */
    const scene = new THREE.Scene()
    const camera = new THREE.PerspectiveCamera(45, width / height, 0.1, 1000)
    camera.position.set(0, 0, 4)

    /* ── lights ───────────────────────────────── */
    scene.add(new THREE.AmbientLight(0xffffff, 0.6))
    const dir = new THREE.DirectionalLight(0xffffff, 1)
    dir.position.set(5, 5, 5)
    scene.add(dir)
    const dir2 = new THREE.DirectionalLight(0xffffff, 0.4)
    dir2.position.set(-3, -2, -4)
    scene.add(dir2)

    /* ── controls ─────────────────────────────── */
    const controls = new OrbitControls(camera, renderer.domElement)
    controls.enableDamping = true
    controls.dampingFactor = 0.08
    controls.enablePan = false

    /* ── build scene content ──────────────────── */
    let mainMesh = null

    const centerAndScale = (geometry) => {
      geometry.computeBoundingBox()
      const box = geometry.boundingBox
      const center = new THREE.Vector3()
      box.getCenter(center)
      geometry.translate(-center.x, -center.y, -center.z)
      const size = new THREE.Vector3()
      box.getSize(size)
      const maxDim = Math.max(size.x, size.y, size.z)
      const scale = 2.5 / maxDim
      geometry.scale(scale, scale, scale)
    }

    const loadSTL = () => {
      const loader = new STLLoader()
      loader.load(
        stlUrl,
        (geometry) => {
          geometry.computeVertexNormals()
          centerAndScale(geometry)

          const mat = new THREE.MeshPhysicalMaterial({
            color,
            metalness: 0.15,
            roughness: 0.35,
            flatShading: true,
          })
          mainMesh = new THREE.Mesh(geometry, mat)
          scene.add(mainMesh)
          setLoading(false)
        },
        undefined,
        (err) => {
          console.error('STL load error', err)
          setError('Failed to load 3D model')
          setLoading(false)
        }
      )
    }

    const loadPLY = () => {
      const loader = new PLYLoader()
      loader.load(
        plyUrl,
        (geometry) => {
          geometry.computeVertexNormals()
          centerAndScale(geometry)

          const hasColors = geometry.hasAttribute('color')
          const mat = new THREE.MeshStandardMaterial({
            vertexColors: hasColors,
            color: hasColors ? 0xffffff : 0xcccccc,
            metalness: 0.05,
            roughness: 0.6,
            flatShading: false,
          })

          mainMesh = new THREE.Mesh(geometry, mat)
          scene.add(mainMesh)

          setLoading(false)
        },
        undefined,
        (err) => {
          console.error('PLY load error', err)
          setError('Failed to load 3D model')
          setLoading(false)
        }
      )
    }

    if (mode === 'stl') {
      loadSTL()
    } else {
      loadPLY()
    }

    /* ── animation loop ───────────────────────── */
    let frameId
    const animate = () => {
      frameId = requestAnimationFrame(animate)
      if (mainMesh && mode === 'stl') {
        mainMesh.rotation.y += 0.004
        mainMesh.rotation.x += 0.001
      }
      controls.update()
      renderer.render(scene, camera)
    }
    animate()

    /* ── resize handling ──────────────────────── */
    const onResize = () => {
      const w = container.clientWidth
      camera.aspect = w / height
      camera.updateProjectionMatrix()
      renderer.setSize(w, height)
    }
    window.addEventListener('resize', onResize)

    /* ── cleanup ──────────────────────────────── */
    return () => {
      window.removeEventListener('resize', onResize)
      cancelAnimationFrame(frameId)
      controls.dispose()
      renderer.dispose()
      if (container.contains(renderer.domElement)) {
        container.removeChild(renderer.domElement)
      }
    }
  }, [mode, stlUrl, plyUrl, color])

  return (
    <div className="model-viewer-wrap">
      <div className="model-viewer" ref={mountRef}>
        {loading && (
          <div className="model-viewer-overlay">
            <span className="model-viewer-spinner">⟳</span>
            <span>Loading 3D model…</span>
          </div>
        )}
        {error && (
          <div className="model-viewer-overlay error">
            <span>{error}</span>
          </div>
        )}
      </div>
      <div className="model-viewer-hint">Click &amp; drag to rotate · Scroll to zoom</div>
      {caption && <span className="image-caption">{caption}</span>}
    </div>
  )
}

export default ModelViewer
