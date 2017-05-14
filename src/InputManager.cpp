////////////////////////////////////////////////////////////////////////////////
/// @file InputManager.cpp
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include "InputManager.h"

InputManager::InputManager (QOpenGLWidget *_window) :
  m_camera(QVector3D(0.0f, 0.0f, -20.0f)),
  m_keys{0},
  m_mousePressed(false)
{
  m_fbo = new QOpenGLFramebufferObject(_window->width(), _window->height());
  // Camera initialisation
  // Must be run on start for camera to calculate its position and orientation
  m_camera.processMouseMovement(0, 0);
}

void InputManager::onHover()
{
  for(auto &s : m_objectList)
  {
    int id = s->compareUniqueColour(m_currentUniqueColour);
    s->setHover(id);
  }
}

void InputManager::doMovement(QVector3D _rp)
{
  // Camera controls
  if(m_keys[Qt::Key_W])
    m_camera.processKeyboard(ARCFORWARD);
  if(m_keys[Qt::Key_S])
    m_camera.processKeyboard(ARCBACKWARD);
  if(m_keys[Qt::Key_A])
    m_camera.processKeyboard(ARCLEFT);
  if(m_keys[Qt::Key_D])
    m_camera.processKeyboard(ARCRIGHT);

  m_view.setToIdentity();
  m_view = m_camera.getViewMatrix();

  m_camera.setRotationPoint(_rp);
}

void InputManager::getUniqueColour(const int _x, const int _y)
{
  m_fbo->bind();
  // Clear colour buffer for temporary drawing
  // This must be run before actual drawing to frame buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Draw all objects in a unique colour
  for(auto &s : m_objectList) { s->drawBackBuffer(); }

  QImage img = m_fbo->toImage();
//  img.save("/home/i7243466/Desktop/koko.jpg");

  QColor col = img.pixelColor(_x, _y);
  setCurrentDepth(img.depth());
  //qDebug("%d %d %d", col.red(), col.green(), col.blue());

  QVector3D pixelColour = QVector3D(col.red(), col.green(), col.blue());
  setCurrentUniqueColour(pixelColour);
  m_fbo->release();
}

void InputManager::doSelection(const int _x, const int _y)
{
  // Compare colours
  for(auto &s : m_objectList)
  {
    QOpenGLShaderProgram* m_manipulatorProgram;
    s->getMainProgram(&m_manipulatorProgram);
    getUniqueColour(_x, _y);
    s->setClicked(m_currentUniqueColour, true);
  }
}

void InputManager::addShaderProgram(QOpenGLShaderProgram* _program)
{
  m_programs.push_back(_program);
}



void InputManager::setupCamera(float _fov, int _w, int _h, float _near, float _far)
{
  //////////////////////////////////////////////////////////////////////////////
  /// CAMERA SETUP
  //////////////////////////////////////////////////////////////////////////////
  m_projection.setToIdentity();
  m_projection.perspective(_fov, (float)_w / (float)_h,
                         _near, _far);
}

void InputManager::loadLightMatricesToShader()
{
  for(size_t i = 0; i < m_programs.size(); i++)
  {
    QOpenGLShaderProgram *prgrm = m_programs[i];
    prgrm->bind();

    prgrm->setUniformValue("view", m_view);
    // Projection matrix only has to be set once, but it makes more
    // sense to have all the setup here.
    prgrm->setUniformValue("projection", m_projection);
    prgrm->release();
  }

}

void InputManager::setObjectList(
    std::vector<std::shared_ptr<SelectableObject> > _objectList)
{
  m_objectList=_objectList;
}

QVector3D InputManager::getCurrentUniqueColour()
{
  return m_currentUniqueColour;
}


void InputManager::setCurrentUniqueColour(QVector3D _uc)
{
  m_currentUniqueColour=_uc;
}

void InputManager::setCurrentDepth(GLfloat _depth)
{
  m_currentDepth=_depth;
}

QMatrix4x4 InputManager::getProjectionMatrix()
{
  return m_projection;
}

QMatrix4x4 InputManager::getViewMatrix()
{
  return m_view;
}

QVector3D InputManager::getCameraPosition()
{
  return m_camera.getPosition();
}

void InputManager::mouseReleaseEvent(QMouseEvent*)
{
  m_mousePressed=false;
  for(auto &s : m_objectList)
  {
    // Set all objects clicked state to false
    s->setClicked(QVector3D(0,0,0), false);
  }
}

void InputManager::mousePressEvent(QMouseEvent *_event)
{
  m_mousePressed=true;

  if(m_alt_key==false)
  {
    doSelection(_event->pos().x(), _event->pos().y());
  }
}


void InputManager::mouseMoveEvent(QMouseEvent *_event)
{
  GLfloat xpos = _event->pos().x();
  GLfloat ypos = _event->pos().y();
  GLfloat xoffset = xpos - m_lastX;
  GLfloat yoffset = m_lastY - ypos;
  getUniqueColour(xpos, ypos);

  // Only process movement if the mouse button and alt is pressed
  if (m_mousePressed && m_alt_key==true)
  {
    m_camera.processMouseMovement(xoffset, yoffset);
    m_view.setToIdentity();
    m_view = m_camera.getViewMatrix();
  }
  else if(m_mousePressed)
  {
    for(auto &s : m_objectList)
    {
      // Get manipulator positions
      QVector3D mp = s->getPosition();
      m_clickZ=mp.z();
      float zoffset=xoffset;
      // Process mouse movement in light class
      s->processMouseMovement(
            xoffset,
            yoffset,
            zoffset,
            m_view,
            m_projection);
    }
  }

  // HOVER
  for(auto &s : m_objectList)
  {
    if(!m_mousePressed)
    {
      QOpenGLShaderProgram* m_manipulatorProgram;
      s->getMainProgram(&m_manipulatorProgram);
      onHover();
    }
  }

  m_lastX = xpos;
  m_lastY = ypos;
}

void InputManager::keyPressEvent(QKeyEvent *_key)
{
  if ( (_key->key() == Qt::Key_Enter)  ||
       (_key->key() == Qt::Key_W)      ||
       (_key->key() == Qt::Key_A)      ||
       (_key->key() == Qt::Key_S)      ||
       (_key->key() == Qt::Key_D)      ||
       (_key->key() == Qt::Key_Return)   )
  {
    m_keys[_key->key()]=true;
  }
  else if(_key->key() == Qt::Key_Alt)
  {
    m_alt_key=true;
  }
  else if(_key->key() == Qt::Key_F)
  {
    m_camera.refocus();
  }
}

void InputManager::keyReleaseEvent(QKeyEvent *_key)
{

  if ( (_key->key() == Qt::Key_Enter)  ||
       (_key->key() == Qt::Key_W)      ||
       (_key->key() == Qt::Key_A)      ||
       (_key->key() == Qt::Key_S)      ||
       (_key->key() == Qt::Key_D)      ||
       (_key->key() == Qt::Key_Return)   )
  {
     // A mapped key was pressed
     m_keys[_key->key()]=false;

  }
  else if(_key->key() == Qt::Key_Alt)
  {
     m_alt_key=false;
  }
}

void InputManager::wheelEvent(QWheelEvent *_event)
{
  m_camera.processMouseScroll(_event->delta());
  m_view.setToIdentity();
  m_view = m_camera.getViewMatrix();

}

void InputManager::resized(int _w, int _h)
{
  delete m_fbo;
  m_fbo = new QOpenGLFramebufferObject(_w, _h);
}

void InputManager::setLightIconScales(float _lightScale)
{

  for(auto &s : m_objectList)
  {
    s->setLightScale(_lightScale);
  }
}

