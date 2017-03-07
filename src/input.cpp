#include "input.h"


InputManager::InputManager (QWindow *_window) :
    camera(QVector3D(0.0f,0.0f,10.0f)),
    mousePressed(false),
    keys{0}
{
    mWindow=_window;

    // Camera initialisation
    // Must be run on start for camera to calculate its position and orientation
    setupCamera();
    camera.ProcessMouseMovement(0,0);
}



/* Calculate mouse click release */
void InputManager::mouseReleaseEvent(QMouseEvent *event)
{
    mousePressed=false;

    for(auto &s : objectList)
    {

        // Set all objects clicked state to false
        s->setClicked(QVector3D(0,0,0), false);
    }

}

/* Run on mouse down */
void InputManager::mousePressEvent(QMouseEvent *event)
{
    mousePressed=true;

    if(alt_key==false)
    {
        doSelection(event->pos().x(), event->pos().y());
    }

}


/* Run Hover function for all objects currently under the mouse cursor  */
void InputManager::onHover()
{
    for(auto &s : objectList)
    {
        int id = s->compareUniqueColour(currentUniqueColour);
        s->setHover(id);


    }

}

void InputManager::mouseMoveEvent(QMouseEvent* event)
{


    GLfloat xpos = event->pos().x();
    GLfloat ypos = event->pos().y();
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    // Only process movement if the mouse button and alt is pressed
    if (mousePressed && alt_key==true)
    {
        camera.ProcessMouseMovement(xoffset, yoffset);
        view.setToIdentity();
        view = camera.GetViewMatrix();

    }
    else if(mousePressed)
    {
        for(auto &s : objectList)
        {

            // Get camera and manipulator positions
            QVector3D cp = camera.getPosition();
            QVector3D mp = s->getPosition();

            clickZ=mp.z();
            GLfloat localXoffset=xoffset; // This variable should not be needed, but the program acts up without it

            float zoffset=xoffset;

            // Reverse x and z offset if the camera is on the other side of the corresponding axis'
            if(cp.x()>mp.x())
            {
                zoffset=-zoffset;
            }

            if(cp.z()>mp.z())
            {
                localXoffset=-localXoffset;

            }

            // Process mouse movement in light class
            s->processMouseMovement(localXoffset, yoffset, zoffset);






        }

    }
    // HOVER
    for(auto &s : objectList)
    {
        if(!mousePressed)
        {
            QOpenGLShaderProgram* m_manipulatorProgram;
            s->getMainProgram(&m_manipulatorProgram);
            getUniqueColour(xpos, ypos, m_manipulatorProgram);

            onHover();

        }
    }



    lastX = xpos;
    lastY = ypos;




}


// Moves/alters the camera positions based on user input
void InputManager::doMovement()
{
    // DeltaTime is simply one for now
    // Will implement when needed
    float deltaTime=1;

    // Camera controls
    if(keys[Qt::Key_W])
        camera.ProcessKeyboard(ARCFORWARD, deltaTime);
    if(keys[Qt::Key_S])
        camera.ProcessKeyboard(ARCBACKWARD, deltaTime);
    if(keys[Qt::Key_A])
      {
        camera.ProcessKeyboard(ARCLEFT, deltaTime);
      }
    if(keys[Qt::Key_D])
      {
        camera.ProcessKeyboard(ARCRIGHT, deltaTime);
      }

    view.setToIdentity();
    view = camera.GetViewMatrix();
}

void InputManager::keyPressEvent(QKeyEvent *key)
{
    if ( (key->key() ==Qt::Key_Enter) ||
         (key->key() ==Qt::Key_W) ||
         (key->key() ==Qt::Key_A) ||
         (key->key() ==Qt::Key_S) ||
         (key->key() ==Qt::Key_D) ||
         (key->key()==Qt::Key_Return) ) {
        keys[key->key()]=true;



    }
    else if(key->key() ==Qt::Key_Alt)
    {
        alt_key=true;

    }

    else if(key->key() ==Qt::Key_F)
    {
      camera.Refocus();
    }


}


 void InputManager::keyReleaseEvent(QKeyEvent *key)
 {

     if ( (key->key() ==Qt::Key_Enter) ||
          (key->key() ==Qt::Key_W) ||
          (key->key() ==Qt::Key_A) ||
          (key->key() ==Qt::Key_S) ||
          (key->key() ==Qt::Key_D) ||
          (key->key()==Qt::Key_Return) )
     {
         // A mapped key was pressed
         keys[key->key()]=false;

     }
     else if(key->key() ==Qt::Key_Alt)
     {
         alt_key=false;

     }


 }

 void InputManager::wheelEvent(QWheelEvent *event)
 {
     camera.ProcessMouseScroll(event->delta());
     view.setToIdentity();
     view = camera.GetViewMatrix();

 }


void InputManager::getUniqueColour(const int _x, const int _y, QOpenGLShaderProgram* m_manipulatorProgram)
{

    // Clear colour buffer for temporary drawing
    // This must be run before actual drawing to frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Draw all objects in a unique colour
    for(auto &s : objectList) { s->drawBackBuffer(); }
    // Pixel storing colour information
    std::array<unsigned char, 3> pixel;

    // Get viewport
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);

    // Read pixel at 1 by 1 pixels
    glReadPixels(_x* mWindow->devicePixelRatio(), viewport[3] - _y*mWindow->devicePixelRatio(),
            1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);
    QVector3D pixelColour = QVector3D(pixel[0], pixel[1], pixel[2]);
    setCurrentUniqueColour(pixelColour);


}


void InputManager::doSelection(const int _x, const int _y)
{
    // Compare colours
    for(auto &s : objectList)
    {
        QOpenGLShaderProgram* m_manipulatorProgram;
        s->getMainProgram(&m_manipulatorProgram);
        getUniqueColour(_x, _y, m_manipulatorProgram);
        s->setClicked(currentUniqueColour, true);

    }


}


void InputManager::addShaderProgram(QOpenGLShaderProgram* _program)
{
    programs.push_back(_program);
}



void InputManager::setupCamera()
{

    int screenWidth=720;
    int screenHeight=480;

    projection.setToIdentity();
    projection.perspective(45, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);

    for(int i=0;i<programs.size();i++)
    {

        QOpenGLShaderProgram *prgrm = programs[i];
        // CAMERA SETUP
        prgrm->bind();

        // Get the uniform locations
        GLint viewLoc = prgrm->uniformLocation("view");
        GLint projLoc = prgrm->uniformLocation("projection");
        // Pass the matrices to the shader
        prgrm->setUniformValue(viewLoc, view);
        prgrm->setUniformValue(projLoc, projection);
        prgrm->release();

    }


}




