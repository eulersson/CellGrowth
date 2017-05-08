////////////////////////////////////////////////////////////////////////////////
/// @file InputManager.cpp
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include "InputManager.h"

InputManager::InputManager (QOpenGLWidget *_window) :
  m_camera(QVector3D(0.0f, 0.0f, -20.0f)),
  m_window(_window),
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

void InputManager::doMovement()
{
  // DeltaTime is simply one for now
  // Will implement when needed
  float deltaTime = 1;

  // Camera controls
  if(m_keys[Qt::Key_W])
    m_camera.processKeyboard(ARCFORWARD, deltaTime);
  if(m_keys[Qt::Key_S])
    m_camera.processKeyboard(ARCBACKWARD, deltaTime);
  if(m_keys[Qt::Key_A])
    m_camera.processKeyboard(ARCLEFT, deltaTime);
  if(m_keys[Qt::Key_D])
    m_camera.processKeyboard(ARCRIGHT, deltaTime);

  m_view.setToIdentity();
  m_view = m_camera.getViewMatrix();
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

  QColor col = img.pixelColor(_x, _y);

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


void InputManager::setupCamera(int _w, int _h)
{
  m_projection.setToIdentity();
  m_projection.perspective(45.0f, (float)_w / (float)_h, 0.1f, 50.0f);

  for(int i = 0; i < m_programs.size(); i++)
  {
    QOpenGLShaderProgram *prgrm = m_programs[i];

    // CAMERA SETUP
    prgrm->bind();

    prgrm->setUniformValue("view", m_view);
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

void InputManager::mouseReleaseEvent(QMouseEvent *_event)
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
      // Get camera and manipulator positions
      QVector3D cp = m_camera.getPosition();
      QVector3D mp = s->getPosition();

      m_clickZ=mp.z();
      GLfloat localXoffset=xoffset; // This variable should not be needed, but the program acts up without it
      float zoffset=xoffset;

      // Process mouse movement in light class
      s->processMouseMovement(localXoffset, yoffset, zoffset, cp, m_view);
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

