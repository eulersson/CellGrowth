#include "openglwindow.h"


// OpenGL
#include <GL/glut.h>
#include <QOpenGLShaderProgram>




OpenGLWindow::OpenGLWindow()
    : screenWidth (720)
    , screenHeight (480)
    , inputManager(this)
    , m_manipulatorProgram(0)

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

//    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject(m_context);

    m_manipulatorProgram = new QOpenGLShaderProgram(this);
    m_manipulatorProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, manipV_SSource);
    m_manipulatorProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, manipF_SSource);
    m_manipulatorProgram->link();

    m_sunProgram = new QOpenGLShaderProgram(this);
    m_sunProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, sunVERTsource);
    m_sunProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, sunFRAGsource);
    m_sunProgram->link();

    QVector3D masterUniqueColour=QVector3D(0.0f, 100.0f, 0.0f);
    for(int x=-4;x<4;x+=4) {
        for(int y=-4;y<4;y+=4) {
            PointLight *pointlight;
            pointlight = new PointLight(QVector3D(x,y,0), m_manipulatorProgram, m_sunProgram);
            pointlight->createGeometry(m_context, masterUniqueColour);
            objectList.push_back(std::move(std::unique_ptr<PointLight>(pointlight)));
        }

    }


    inputManager.addShaderProgram(m_manipulatorProgram);
    inputManager.addShaderProgram(m_sunProgram);
    inputManager.setObjectList(objectList);




}



void OpenGLWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // DRAW MANIPULATORS
    inputManager.setupCamera();
    inputManager.doMovement();
    for(auto &s : objectList)
    {
        s->draw();
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
// QVector3D OpenGLWindow::unProject(float x, float y, float z)
// {



//    glm::uvec4 viewport(0.0f,0.0f,(float)screenWidth, (float)screenHeight);

//    glm::vec3 projected=glm::project(glm::vec3(x,y,0), model*view, projection, viewport);
//    //glm::vec3 unProjected= glm::unProject(glm::vec3(x,y,0), model*view, projection, viewport);


//    // Get the final mvp matrix
//    glm::mat4 finm=view*model*projection;


//    float *fm=(float*)glm::value_ptr(finm);
//    float posZ=-z; // Modify z to have movement speed increase and decrease with distance from camera
//    // Ideally manipulator should always stick to the cursor as much as possible

//    //qDebug() << QString::number(fm[8]);

////        QMatrix4x4 inverted(fm[0],  fm[1],  fm[2],  fm[3],
////                            fm[4],  fm[5],  fm[6],  fm[7],
////                            fm[8],  fm[9],  fm[10], fm[11],
////                            fm[12], fm[13], fm[14], fm[15]);


//    QMatrix4x4 inverted(fm[0], fm[4], fm[8], fm[12],
//                        fm[1], fm[5], fm[9], fm[13],
//                        fm[2], fm[6], fm[10], fm[14],
//                        fm[3], fm[7], fm[11], fm[15]);

//    // Get the inverted of the matrix
//    inverted=inverted.inverted();

//    float posY = screenHeight - y - 1.0f;
//    posY=y;

//    //QVector4D clickedPointOnScreen(x, posY, 2.0f * posZ - 1.0f, 1.0f);
//    QVector4D clickedPointOnScreen(x, posY, posZ , 1.0f);
//    QVector4D clickedPointIn3DOrgn = inverted * clickedPointOnScreen;

//    clickedPointIn3DOrgn /= clickedPointIn3DOrgn.w();

//    // Return 3D vector from 4D vector
//    // Subtract samera position to have positions stay consistent
//    return (clickedPointIn3DOrgn.toVector3DAffine());


//    //return QVector3D (unProjected.x, unProjected.y, unProjected.z);


// }

void OpenGLWindow::keyPressEvent(QKeyEvent *key)
{
    inputManager.keyPressEvent(key);
}

void OpenGLWindow::keyReleaseEvent(QKeyEvent *key)
{
    inputManager.keyReleaseEvent(key);
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
    inputManager.mouseMoveEvent(event);
}

void OpenGLWindow::mousePressEvent(QMouseEvent *event)
{
    inputManager.mousePressEvent(event);
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event)
{
    inputManager.mouseReleaseEvent(event);
}

void OpenGLWindow::wheelEvent(QWheelEvent *event)
{
    inputManager.wheelEvent(event);
}






