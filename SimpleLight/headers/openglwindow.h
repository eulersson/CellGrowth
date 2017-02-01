#include <iostream>
#include <fstream>
#include <memory>

// QT
#include <QDebug>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>
#include <QWidget>
#include <QKeyEvent>
#include <QTime>


// OpenGL
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

// Custom
#include "viewport.h"
#include "manipulator.h"
#include "camera.h"

class OpenGLWindow : public Viewport
{
    Q_OBJECT
public:
    explicit OpenGLWindow();
    ~OpenGLWindow();

//    virtual void render(QPainter *painter);
    void render() Q_DECL_OVERRIDE;

    void initialize() Q_DECL_OVERRIDE;

    void Update();

    void Do_Movement();
    void keyPressEvent(QKeyEvent *key);
    void keyReleaseEvent(QKeyEvent *key);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void doSelection(const int _x, const int _y);

    void createGeometryPoints(std::vector<QVector3D>& vertices,
                           float radius,
                           unsigned int rings,
                           unsigned int sectors);

    std::string readFile(const char *filePath);
    QVector3D unProject(float x, float y);

    QMatrix4x4 perspective(float fovy, float apsect, float zNear, float zFar);

    //void setAnimating(bool animating);

public slots:
    //void renderLater();
//    void renderNow();

protected:
//    bool event(QEvent *event) Q_DECL_OVERRIDE;

    //void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    //bool m_update_pending;
    //bool m_animating;

    int screenWidth, screenHeight;
    Camera camera;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    // Array of bools stating if a key is pressed or not
    // All keys have a custom int value
    bool keys[1024]; // 1024
    bool alt_key=false;
    // Variable stating wether or not the mouse is currently pressed
    bool mousePressed;
    GLfloat lastX = 400, lastY = 300;

    // Array of all scene objects
    std::vector<std::unique_ptr<SelectObject>> objectList;

    Manipulator *manip;



    // QT IMPLEMENTATION
    unsigned int m_numberOfPoints;

    std::vector<QVector3D> m_pointPos;
    std::vector<GLfloat> m_pointPosArray;

    // We will buffer m_particlePosArray (a GLfloat array) to this VBO
    QOpenGLBuffer* vbo;

    // This VAO is used to draw the particles
    QOpenGLVertexArrayObject* vao;

    QOpenGLShaderProgram *m_lightProgram;

    QOpenGLContext *m_context;


//    QOpenGLPaintDevice *m_device;
};
