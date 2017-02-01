#include "openglwindow.h"

// OpenGL
#include <GL/glut.h>
#include <QOpenGLShaderProgram>





OpenGLWindow::OpenGLWindow()
    :  m_lightProgram(0),
      screenWidth (720),
      screenHeight (480),
      camera(glm::vec3(0.0f,0.0f,10.0f)),
      mousePressed(false),
      keys{0}
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
    std::string vertexShaderString = readFile("shaders/vertex.glsl");
    std::string fragmentShaderString = readFile("shaders/fragment.glsl");
    const char *vertexShaderSource = vertexShaderString.c_str();
    const char *fragmentShaderSource = fragmentShaderString.c_str();

    // GLUseProgram?
    m_lightProgram = new QOpenGLShaderProgram(this);
    m_lightProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_lightProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_lightProgram->link();

    vao = new QOpenGLVertexArrayObject(this);

    m_pointPos = std::vector<QVector3D>(0);
    manip = new Manipulator(QVector3D(0,0,0), m_lightProgram);



    // Bind 3 vbos for the 3 arrows
    m_numberOfPoints=0;

    // X ARROW
    QOpenGLVertexArrayObject *vao_x = new QOpenGLVertexArrayObject(this);
    manip->createGeometry(vao_x, QVector3D(.4,0,0), 0);
    // Add to count of number of vertices
    //m_numberOfPoints+=m_pointPos.size();

    // Y ARROW
    QOpenGLVertexArrayObject *vao_y = new QOpenGLVertexArrayObject(this);
    manip->createGeometry(vao_y, QVector3D(0,.4,0), 1);
    // Add to count of number of vertices
    //m_numberOfPoints+=m_pointPos.size();

    // Z ARROW
    QOpenGLVertexArrayObject *vao_z = new QOpenGLVertexArrayObject(this);
    manip->createGeometry(vao_z, QVector3D(0,0,.4), 2);
    // Add to count of number of vertices
    m_numberOfPoints+=m_pointPos.size()*2;

    //m_numberOfPoints=m_pointPos.size();


    objectList.push_back(std::move(std::unique_ptr<Manipulator>(manip)));



//    // OpenGL wants a flat array of GLfloats
//    m_numberOfPoints = m_pointPos.size();
//    for (unsigned int i = 0; i < m_numberOfPoints; i++)
//    {


//        m_pointPosArray.push_back(m_pointPos[i].x());
//        m_pointPosArray.push_back(m_pointPos[i].y());
//        m_pointPosArray.push_back(m_pointPos[i].z());
//    }

    // Initialize VAO and VBO Qt objects
    //vao = new QOpenGLVertexArrayObject(this);
//    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);


//    // Buffer
//    vbo->create();
//    vbo->bind();
//    vbo->setUsagePattern(QOpenGLBuffer::StaticDraw); //Previoulsy DynamicDraw
//    vbo->allocate(&m_pointPosArray[0], 3*m_numberOfPoints * sizeof(GLfloat));


    // Vertex Array Object
    vao->create();
    vao->bind();




    // Stride is 5* size of float because the vertex contains 6 values
    // First number states where to start (offset)
    // Second number states the size of the data to get (position = x,y,z = 3 values)
    // Third number is stride. Must be multiplied with sizeof float for some reason


//    m_lightProgram->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3);
//    m_lightProgram->enableAttributeArray("posAttr");

    m_lightProgram->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6*sizeof(GL_FLOAT));
    m_lightProgram->enableAttributeArray("posAttr");

    m_lightProgram->setAttributeBuffer("normAttr", GL_FLOAT, 3* sizeof(GLfloat), 3, 6*sizeof(GL_FLOAT));
    m_lightProgram->enableAttributeArray("normAttr");





    vao->release();






}



void OpenGLWindow::render()
{



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    m_lightProgram->bind();
//    vao->bind();



    // CAMERA SETUP
    // Create camera transformation
    view = camera.GetViewMatrix();
    projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 1000.0f);
    // Get the uniform locations
    GLint modelLoc = m_lightProgram->uniformLocation("model"); //glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = m_lightProgram->uniformLocation("view");
    GLint projLoc = m_lightProgram->uniformLocation("projection");
    // Pass the matrices to the shader
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view(0,0));
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection(0,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


    QVector3D manipPos=manip->getPosition();
    model=glm::mat4();
    model = glm::translate(model, glm::vec3(manipPos.x(), manipPos.y(), manipPos.z()));
    //model(3)=
    GLfloat angle = 0.0f;
    model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model(0,0));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    // CAMERA DONE


    Do_Movement();



    QVector3D baseColour(0.0f, 0.0f, 1.0f);
    m_lightProgram->setUniformValue("baseColour", baseColour);


    // DRAWING
    //glEnable(GL_POINT_SPRITE);
    //glPointSize(12.0f);

    //glDrawArrays(GL_TRIANGLES, 0, m_numberOfPoints); // Previously GL_POINTS
    manip->draw();


    // Unbound (release)
//    vao->release();
    m_lightProgram->release();




//    for (unsigned int i = 0; i < m_numberOfPoints; i++)
//    {
//        m_pointPosArray.push_back(m_pointPos[i].x());
//        m_pointPosArray.push_back(m_pointPos[i].y());
//        m_pointPosArray.push_back(m_pointPos[i].z());
//    }

//    m_VBO_pointPos->bind();

//    m_VBO_pointPos->write(0, &m_pointPosArray[0], m_numberOfPoints * 3 * sizeof(GLfloat));
//    m_VBO_pointPos->release();



}



void OpenGLWindow::doSelection(const int _x, const int _y)
{

    // Clear colour buffer for temporary drawing
    // This must be run before actual drawing to frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    m_lightProgram->bind();
    vao->bind();


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


    // Compare colours
    for(auto &s : objectList)
    {
        if(s->compareUniqueColour(pixelColour))
        {
            //qDebug() << QString::number(pixel[0]);

            s->setClicked(pixelColour, true);


        }


    }



    // Draw correct image
    render();

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

 QVector3D OpenGLWindow::unProject(float x, float y)
 {

    glm::uvec4 viewport(0.0f,0.0f,(float)screenWidth, (float)screenHeight);

    glm::vec3 projected=glm::project(glm::vec3(x,y,0), model*view, projection, viewport);
    //glm::vec3 unProjected= glm::unProject(glm::vec3(x,y,0), model*view, projection, viewport);

    glm::mat4 finm=view*model*projection;


    float *fm=(float*)glm::value_ptr(finm);
    float posZ=0.2f;

    //qDebug() << QString::number(fm[8]);

//    QMatrix4x4 inverted(fm[0],  fm[1],  fm[2],  fm[3],
//                        fm[4],  fm[5],  fm[6],  fm[7],
//                        fm[8],  fm[9],  fm[10], fm[11],
//                        fm[12], fm[13], fm[14], fm[15]);

    QMatrix4x4 inverted(fm[0], fm[4], fm[8], fm[12],
                        fm[1], fm[5], fm[9], fm[13],
                        fm[2], fm[6], fm[10], fm[14],
                        fm[3], fm[7], fm[11], fm[15]);

    inverted=inverted.inverted();

    float posY = screenHeight - y - 1.0f;
    //posY=y;

    //QVector4D clickedPointOnScreen(x, posY, 2.0f * posZ - 1.0f, 1.0f);
    QVector4D clickedPointOnScreen(-x, posY, posZ , 1.0f);
    QVector4D clickedPointIn3DOrgn = inverted * clickedPointOnScreen;

    clickedPointIn3DOrgn /= clickedPointIn3DOrgn.w();

    // Return 3D vector from 4D vector
    return clickedPointIn3DOrgn.toVector3DAffine();


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
     // Always update movement to avoid camera jumps
     if (mousePressed && alt_key==true)
     {

         camera.ProcessMouseMovement(xoffset, yoffset);



     }
     else if(mousePressed)
     {
         for(auto &s : objectList)
         {

             QVector3D unProjectedLast = unProject(lastX, lastY);
             QVector3D unProjected=unProject(xpos, ypos);



             xoffset = unProjected.x() - unProjectedLast.x();
             yoffset = unProjectedLast.y() - unProjected.y();

             s->processMouseMovement(xoffset, yoffset);


         }

     }

//     qDebug() << QString::number(event->pos().x());
//     qDebug() << QString::number(lastX);
//     qDebug() << QString::number(xoffset);
//     qDebug() << QString::number(yoffset);

//     qDebug("line break---");


     lastX = xpos;
     lastY = ypos;



 }

 void OpenGLWindow::mousePressEvent(QMouseEvent *event)
 {
     mousePressed=true;

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






