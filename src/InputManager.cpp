////////////////////////////////////////////////////////////////////////////////
/// @file InputManager.cpp
/// @author Glenn Nygard
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include "InputManager.h"

InputManager::InputManager (QOpenGLWidget *_window) :
  m_camera(QVector3D(0.0f, 0.0f, 10.0f)),
  m_window(_window),
  m_keys{0},
  m_mousePressed(false)
{
    // Camera initialisation
    // Must be run on start for camera to calculate its position and orientation
    setupCamera();
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

void InputManager::getUniqueColour(const int _x, const int _y,
                                   QOpenGLShaderProgram* m_manipulatorProgram)
{
  // Clear colour buffer for temporary drawing
  // This must be run before actual drawing to frame buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Draw all objects in a unique colour
  for(auto &s : m_objectList) { s->drawBackBuffer(); }
  // Pixel storing colour information
  std::array<unsigned char, 3> pixel;

  // Get viewport
  std::array<GLint, 4> viewport;
  glGetIntegerv(GL_VIEWPORT, &viewport[0]);

  // Read pixel at 1 by 1 pixels
  glReadPixels(
    _x* m_window->devicePixelRatio(),
    viewport[3] - _y*m_window->devicePixelRatio(),
    1,
    1,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    &pixel[0]);

  QVector3D pixelColour = QVector3D(pixel[0], pixel[1], pixel[2]);
  setCurrentUniqueColour(pixelColour);
}

void InputManager::doSelection(const int _x, const int _y)
{
  // Compare colours
  for(auto &s : m_objectList)
  {
    QOpenGLShaderProgram* m_manipulatorProgram;
    s->getMainProgram(&m_manipulatorProgram);
    getUniqueColour(_x, _y, m_manipulatorProgram);
    s->setClicked(m_currentUniqueColour, true);
  }
}

void InputManager::addShaderProgram(QOpenGLShaderProgram* _program)
{
  m_programs.push_back(_program);
}

void InputManager::setupCamera()
{
  int screenWidth = 720;
  int screenHeight = 480;

  m_projection.setToIdentity();
  m_projection.perspective(45, (float)screenWidth / (float)screenHeight,
                         0.1f, 1000.0f);

  for(int i = 0; i < m_programs.size(); i++)
  {
    QOpenGLShaderProgram *prgrm = m_programs[i];

    // CAMERA SETUP
    prgrm->bind();

    // Get the uniform locations
    GLint viewLoc = prgrm->uniformLocation("view");
    GLint projLoc = prgrm->uniformLocation("projection");

    // Pass the matrices to the shader
    prgrm->setUniformValue(viewLoc, m_view);
    prgrm->setUniformValue(projLoc, m_projection);
    prgrm->release();
  }
}

void InputManager::setObjectList(
    std::vector<std::shared_ptr<SelectObject> > _objectList)
{
  m_objectList=_objectList;
}

QVector3D InputManager::getCurrentUniqueColour()
{
  return m_currentUniqueColour;
}

void InputManager::setCurrentUniqueColour(QVector3D uc)
{
  m_currentUniqueColour=uc;
}

QMatrix4x4 InputManager::getProjectionMatrix()
{
  return m_projection;
}

QMatrix4x4 InputManager::getViewMatrix()
{
  return m_view;
}

void InputManager::mouseReleaseEvent(QMouseEvent *event)
{
  m_mousePressed=false;
  for(auto &s : m_objectList)
  {
    // Set all objects clicked state to false
    s->setClicked(QVector3D(0,0,0), false);
  }
}

void InputManager::mousePressEvent(QMouseEvent *event)
{
  m_mousePressed=true;

  if(m_alt_key==false)
  {
    doSelection(event->pos().x(), event->pos().y());
  }
}

void InputManager::mouseMoveEvent(QMouseEvent* event)
{
  GLfloat xpos = event->pos().x();
  GLfloat ypos = event->pos().y();
  GLfloat xoffset = xpos - m_lastX;
  GLfloat yoffset = m_lastY - ypos;

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

      // Reverse x and z offset if the camera is on the other side of the corresponding axis'
      if(cp.x()>mp.x())
      {
        zoffset=-zoffset;
      }

      if(cp.z()>mp.z()){
        localXoffset=-localXoffset;
      }

      // Process mouse movement in light class
      s->processMouseMovement(localXoffset, yoffset, zoffset);
    }
  }

  // HOVER
  for(auto &s : m_objectList)
  {
    if(!m_mousePressed)
    {
      QOpenGLShaderProgram* m_manipulatorProgram;
      s->getMainProgram(&m_manipulatorProgram);
      getUniqueColour(xpos, ypos, m_manipulatorProgram);

      onHover();
    }
  }

  m_lastX = xpos;
  m_lastY = ypos;
}

void InputManager::keyPressEvent(QKeyEvent *key)
{
  if ( (key->key() == Qt::Key_Enter)  ||
       (key->key() == Qt::Key_W)      ||
       (key->key() == Qt::Key_A)      ||
       (key->key() == Qt::Key_S)      ||
       (key->key() == Qt::Key_D)      ||
       (key->key() == Qt::Key_Return)   )
  {
    m_keys[key->key()]=true;
  }
  else if(key->key() == Qt::Key_Alt)
  {
    m_alt_key=true;
  }
  else if(key->key() == Qt::Key_F)
  {
    m_camera.refocus();
  }
}

void InputManager::keyReleaseEvent(QKeyEvent *key)
{

  if ( (key->key() == Qt::Key_Enter)  ||
       (key->key() == Qt::Key_W)      ||
       (key->key() == Qt::Key_A)      ||
       (key->key() == Qt::Key_S)      ||
       (key->key() == Qt::Key_D)      ||
       (key->key() == Qt::Key_Return)   )
  {
     // A mapped key was pressed
     m_keys[key->key()]=false;

  }
  else if(key->key() == Qt::Key_Alt)
  {
     m_alt_key=false;
  }
}

void InputManager::wheelEvent(QWheelEvent *event)
{
  m_camera.processMouseScroll(event->delta());
  m_view.setToIdentity();
  m_view = m_camera.getViewMatrix();
}
