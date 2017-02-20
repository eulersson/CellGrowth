#include "openglwindow.h"


// OpenGL
#include <GL/glut.h>
#include <QOpenGLShaderProgram>



// Z position of clicked item
// A global variable to make sure the z position is only gotten on mouse click
// not mouse down (continous)
float clickZ;





OpenGLWindow::OpenGLWindow()
    :  m_manipulatorProgram(0),
      screenWidth (720),
      screenHeight (480),
      camera(glm::vec3(0.0f,0.0f,10.0f)),
      mousePressed(false),
      keys{0},
      inputManager(this)
{


    // Set window format
    QSurfaceFormat format;
    format.setSamples(16);
    this->resize(screenWidth,screenHeight);
    this->setFormat(format);


    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow()
{

}

/*
 * Function used to read shaders from file.
 * Returns string of shader.
 *
 * */
std::string OpenGLWindow::readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

// Passing in address of vertices because they will get mutated
void OpenGLWindow::createGeometryPoints(std::vector<QVector3D>& vertices,
                       float radius,
                       unsigned int rings,
                       unsigned int sectors)
{
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(unsigned int r = 0; r < rings; ++r) {
        for(unsigned int s = 0; s < sectors; ++s) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            vertices.push_back(QVector3D(x,y,z) * radius);
        }
    }


}

void OpenGLWindow::initialize()
{


    glViewport(0, 0, width(), height());
    // Enable GL depth
    glEnable(GL_DEPTH_TEST);

    // Get shaders
    std::string manipV_SS = readFile("shaders/manipvert.glsl");
    std::string manipF_SS = readFile("shaders/manipfrag.glsl");
    const char *manipV_SSource = manipV_SS.c_str();
    const char *manipF_SSource = manipF_SS.c_str();
    std::string sunVERTstring = readFile("shaders/sunvert.glsl");
    std::string sunFRAGstring = readFile("shaders/sunfrag.glsl");
    const char *sunVERTsource = sunVERTstring.c_str();
    const char *sunFRAGsource = sunFRAGstring.c_str();


    m_manipulatorProgram = new QOpenGLShaderProgram(this);
    m_manipulatorProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, manipV_SSource);
    m_manipulatorProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, manipF_SSource);
    m_manipulatorProgram->link();

    m_sunProgram = new QOpenGLShaderProgram(this);
    m_sunProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, sunVERTsource);
    m_sunProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, sunFRAGsource);
    m_sunProgram->link();


    // Stride is 5* size of float because the vertex contains 6 values
    // First number states where to start (offset)
    // Second number states the size of the data to get (position = x,y,z = 3 values)
    // Third number is stride. Must be multiplied with sizeof float for some reason
    pointlight = new PointLight(QVector3D(0,0,0), m_manipulatorProgram, m_sunProgram);
    pointlight->createGeometry(m_context);
    objectList.push_back(std::move(std::unique_ptr<PointLight>(pointlight)));






}



void OpenGLWindow::render()
{



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    // DRAW MANIPULATORS

    setupCamera();
    Do_Movement();


    pointlight->draw();






}

void OpenGLWindow::setupCamera()
{

    // CAMERA SETUP
    m_manipulatorProgram->bind();
    // Create camera transformation
    view = camera.GetViewMatrix();
    projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
    // Get the uniform locations
    GLint modelLoc = m_manipulatorProgram->uniformLocation("model");
    GLint viewLoc = m_manipulatorProgram->uniformLocation("view");
    GLint projLoc = m_manipulatorProgram->uniformLocation("projection");
    // Pass the matrices to the shader

    QVector3D pointPos=pointlight->getPosition();
    model=glm::mat4();
    model = glm::translate(model, glm::vec3(pointPos.x(), pointPos.y(), pointPos.z()));
    GLfloat angle = 0.0f;
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    m_manipulatorProgram->release();


    m_sunProgram->bind();
    modelLoc = m_sunProgram->uniformLocation("model");
    viewLoc = m_sunProgram->uniformLocation("view");
    projLoc = m_sunProgram->uniformLocation("projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    m_sunProgram->release();

}



void OpenGLWindow::doSelection(const int _x, const int _y)
{

    // Clear colour buffer for temporary drawing
    // This must be run before actual drawing to frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    m_manipulatorProgram->bind();
    //vao->bind();


    // Draw all objects in a unique colour
    for(auto &s : objectList)
    {
        //m_lightProgram->setUniformValue("baseColour", s->getUniqueColour());

        //glDrawArrays(GL_TRIANGLES, 0, m_numberOfPoints);
        s->drawBackBuffer();



    }


    // Pixel storing colour information
    std::array<unsigned char, 3> pixel;

    // Get viewport
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    // Read pixel at 1 by 1 pixels
    glReadPixels(_x*devicePixelRatio(), viewport[3] - _y*devicePixelRatio(),
            1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);

    QVector3D pixelColour = QVector3D(pixel[0], pixel[1], pixel[2]);
    inputManager.setCurrentUniqueColour(pixelColour);


    // Compare colours
    for(auto &s : objectList)
    {
        s->setClicked(pixelColour, true);



    }



    // Draw correct image
    render();

}


void OpenGLWindow::getUniqueColour(const int _x, const int _y)
{

    // Clear colour buffer for temporary drawing
    // This must be run before actual drawing to frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    m_manipulatorProgram->bind();
    //vao->bind();


    // Draw all objects in a unique colour
    for(auto &s : objectList)
    {
        s->drawBackBuffer();
    }

    // Pixel storing colour information
    std::array<unsigned char, 3> pixel;
    // Get viewport
    std::array<GLint, 4> viewport;
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    // Read pixel at 1 by 1 pixels
    glReadPixels(_x*devicePixelRatio(), viewport[3] - _y*devicePixelRatio(),
            1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);

    QVector3D pixelColour = QVector3D(pixel[0], pixel[1], pixel[2]);
    inputManager.setCurrentUniqueColour(pixelColour);



}





// Moves/alters the camera positions based on user input
void OpenGLWindow::Do_Movement()
{
    // DeltaTime is simply one for now
    // Will implement later when needed
    float deltaTime=1;

    // Camera controls
    if(keys[Qt::Key_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[Qt::Key_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[Qt::Key_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[Qt::Key_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *key)
{
    if ( (key->key() ==Qt::Key_Enter) ||
         (key->key() ==Qt::Key_W) ||
         (key->key() ==Qt::Key_A) ||
         (key->key() ==Qt::Key_S) ||
         (key->key() ==Qt::Key_D) ||
         (key->key()==Qt::Key_Return) ) {
        //Enter or return was pressed
        keys[key->key()]=true;



    }
    else if(key->key() ==Qt::Key_Alt)
    {
        alt_key=true;

    }
    else {
        // Something else was pressed
    }


}


 void OpenGLWindow::keyReleaseEvent(QKeyEvent *key)
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
     else {
         // Something else was pressed
     }


 }


 /* @brief Unproject a screen position to world position. Z might be needed depending on what you are doing.
  * Main usecase is to make manipulator movement react 1:1 with cursor movement.
  *
  * in float x: x screen position.
  *
  * in float y: y screen position.
  *
  * in float z: z position of object. This value is lost and therefore needs to be gotten some oether way
  *             like through z-depth. Default 1.
  *
  *
  * @return QVector3D Position in 3d space
  */
 QVector3D OpenGLWindow::unProject(float x, float y, float z)
 {



    glm::uvec4 viewport(0.0f,0.0f,(float)screenWidth, (float)screenHeight);

    glm::vec3 projected=glm::project(glm::vec3(x,y,0), model*view, projection, viewport);
    //glm::vec3 unProjected= glm::unProject(glm::vec3(x,y,0), model*view, projection, viewport);


    // Get the final mvp matrix
    glm::mat4 finm=view*model*projection;


    float *fm=(float*)glm::value_ptr(finm);
    float posZ=-z; // Modify z to have movement speed increase and decrease with distance from camera
    // Ideally manipulator should always stick to the cursor as much as possible

    //qDebug() << QString::number(fm[8]);

//        QMatrix4x4 inverted(fm[0],  fm[1],  fm[2],  fm[3],
//                            fm[4],  fm[5],  fm[6],  fm[7],
//                            fm[8],  fm[9],  fm[10], fm[11],
//                            fm[12], fm[13], fm[14], fm[15]);

    QMatrix4x4 inverted(fm[0], fm[4], fm[8], fm[12],
                        fm[1], fm[5], fm[9], fm[13],
                        fm[2], fm[6], fm[10], fm[14],
                        fm[3], fm[7], fm[11], fm[15]);

    // Get the inverted of the matrix
    inverted=inverted.inverted();

    float posY = screenHeight - y - 1.0f;
    posY=y;

    //QVector4D clickedPointOnScreen(x, posY, 2.0f * posZ - 1.0f, 1.0f);
    QVector4D clickedPointOnScreen(x, posY, posZ , 1.0f);
    QVector4D clickedPointIn3DOrgn = inverted * clickedPointOnScreen;

    clickedPointIn3DOrgn /= clickedPointIn3DOrgn.w();

    // Return 3D vector from 4D vector
    // Subtract samera position to have positions stay consistent
    return (clickedPointIn3DOrgn.toVector3DAffine());


    //return QVector3D (unProjected.x, unProjected.y, unProjected.z);


 }


 void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
 {


     GLfloat xpos = event->pos().x();
     GLfloat ypos = event->pos().y();


     // Reversed since y-coordinates go from bottom to left
     GLfloat xoffset = xpos - lastX;
     GLfloat yoffset = lastY - ypos;




     // Only process movement if the mouse button and alt is pressed
     if (mousePressed && alt_key==true)
     {

         camera.ProcessMouseMovement(xoffset, yoffset);



     }
     else if(mousePressed)
     {
         for(auto &s : objectList)
         {

             // Get camera and manipulator positions
             QVector3D cp = camera.getPosition();
             QVector3D mp = s->getPosition();

             clickZ=mp.z();


             //clickZ = abs(camera.getPosition().z() - s->getPosition().z());

//             QVector3D unProjectedLast = unProject(lastX, lastY, clickZ);
//             QVector3D unProjected=unProject(xpos, ypos, clickZ);
//             xoffset = -(unProjected.x() - unProjectedLast.x());
//             yoffset = -(unProjectedLast.y() - unProjected.y());



             float zoffset=xoffset;

             // Reverse x and z offset if the camera is on the other side of the corresponding axis'
             if(cp.x()<mp.x())
             {
                 zoffset=-zoffset;
             }

             if(cp.z()<mp.z())
             {
                 xoffset=-xoffset;
             }


             // Process mouse movement in light class
             s->processMouseMovement(xoffset, yoffset, zoffset);






         }

     }

//     qDebug() << QString::number(event->pos().x());
//     qDebug() << QString::number(lastX);
//     qDebug() << QString::number(xoffset);
//     qDebug() << QString::number(yoffset);

//     qDebug("line break---");


     // HOVER
     if(!mousePressed)
     {
         getUniqueColour(xpos, ypos);
         inputManager.onHover(objectList);

     }




     lastX = xpos;
     lastY = ypos;




 }

 void OpenGLWindow::mousePressEvent(QMouseEvent *event)
 {
     mousePressed=true;

     QVector3D unProjected=unProject(event->pos().x(), event->pos().y(), clickZ);

     qDebug() << QString::number(unProjected.x());
     qDebug() << QString::number(unProjected.y());
     qDebug() << QString::number(unProjected.z());

     qDebug("new line__________");



     if(alt_key==false)
     {
         doSelection(event->pos().x(), event->pos().y());
     }

 }

 void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
 {
     mousePressed=false;

     for(auto &s : objectList)
     {

         // Set all objects clicked state to false
         s->setClicked(QVector3D(0,0,0), false);



     }

 }






