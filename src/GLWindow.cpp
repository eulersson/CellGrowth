#include "include/GLWindow.h"

GLWindow::GLWindow(QWidget *_parent):QOpenGLWidget(_parent)
{
  setFocus();

  // we might not need this
  this->resize(_parent->size());


  //original code
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


