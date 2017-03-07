////////////////////////////////////////////////////////////////////////////////
/// @file Window.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Qt
#include <QKeyEvent>

// Project
#include "Window.h"

Window::Window(QWindow *parent) : QOpenGLWindow(NoPartialUpdate, parent)
{
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));
  if(format().swapInterval() == -1)
  {
      // V_blank synchronization not available (tearing likely to happen)
      qDebug("Swap Buffers at v_blank not available: refresh at approx 60fps.");
      m_timer.setInterval(17);
  }
  else
  {
      // V_blank synchronization available
      m_timer.setInterval(0);
  }
  m_timer.start();
}

AbstractScene *Window::scene() const
{
  return m_scene;
}

void Window::setScene(AbstractScene *_scene)
{
  m_scene = _scene;
}

void Window::initializeGL()
{
  if (scene())
    scene()->initialize();
}

void Window::paintGL()
{
  if (scene())
    scene()->paint();
}

void Window::resizeGL(int _w, int _h)
{
  scene()->windowResized(_w, _h);

}

void Window::keyPressEvent(QKeyEvent* ev)
{
  scene()->keyPressed(ev);
}

void Window::keyReleaseEvent(QKeyEvent *key)
{
  scene()->keyReleaseEvent(key);
}

void Window::mouseMoveEvent(QMouseEvent* event)
{
  scene()->mouseMoveEvent(event);
}

void Window::mousePressEvent(QMouseEvent *event)
{
  scene()->mousePressEvent(event);
}

void Window::mouseReleaseEvent(QMouseEvent *event)
{
  scene()->mouseReleaseEvent(event);
}

void Window::wheelEvent(QWheelEvent *event)
{
  scene()->wheelEvent(event);
}
