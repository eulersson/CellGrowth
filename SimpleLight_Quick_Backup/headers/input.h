/* Input helper class **/

#include <memory>


// QT
#include <QVector3D>
#include <QtGui/QWindow>


// Custom
#include "selectobject.h"
#include "camera.h"





class InputManager
{

public:
    InputManager (QWindow *_window) :
        mousePressed(false),
        keys{0}
    {
        mWindow=_window;


    }

    /* Run Hover function for all objects currently under the mouse cursor  */
    void onHover(std::vector<std::shared_ptr<SelectObject>> objectList)
    {

        for(auto &s : objectList)
        {
            int id = s->compareUniqueColour(currentUniqueColour);
            s->setHover(id);


        }

    }

    /* Calculate mouse click */
    void onMouseClick()
    {

    }

    /* Calculate mouse click release */
    void onMouseRelease()
    {


    }

    /* Calculate mouse down loop */
    void onMouseDown()
    {

    }

    void onMouseMove(QMouseEvent* event, Camera camera, std::vector<std::shared_ptr<SelectObject>> objectList,
                     QOpenGLShaderProgram *m_manipulatorProgram)
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
            getUniqueColour(m_manipulatorProgram, objectList, xpos, ypos);
            onHover(objectList);

        }




        lastX = xpos;
        lastY = ypos;




    }


    void getUniqueColour(QOpenGLShaderProgram *m_manipulatorProgram, std::vector<std::shared_ptr<SelectObject>> objectList,
                         const int _x, const int _y)
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
        glReadPixels(_x* mWindow->devicePixelRatio(), viewport[3] - _y*mWindow->devicePixelRatio(),
                1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel[0]);
        QVector3D pixelColour = QVector3D(pixel[0], pixel[1], pixel[2]);
        setCurrentUniqueColour(pixelColour);

    }




    QVector3D getCurrentUniqueColour() { return currentUniqueColour; }
    void setCurrentUniqueColour(QVector3D uc) { currentUniqueColour=uc; }



private:
    QVector3D currentUniqueColour;

    QWindow *mWindow;


    Camera camera;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;


    // INPUT
    // Array of bools stating if a key is pressed or not
    // All keys have a custom int value
    bool keys[1024]; // 1024
    bool alt_key=false;
    // Variable stating wether or not the mouse is currently pressed
    bool mousePressed;
    GLfloat lastX = 400, lastY = 300;
    GLfloat clickZ;



};




