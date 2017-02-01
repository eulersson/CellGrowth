#include <QtGui/QGuiApplication>
#include "openglwindow.h"

QTime lastUpdateTime = QTime::currentTime();
int updateTimeStep = 5;

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    // Create and display window
    OpenGLWindow mainWindow;
    mainWindow.show();
    mainWindow.setAnimating(true); // Render function will be continously called


//    while(true)
//    {
//        app.processEvents();
//        QTime currentTime= QTime::currentTime();
//        int timeSinceLastUpdate = lastUpdateTime.msecsTo(currentTime);
//        while(timeSinceLastUpdate>updateTimeStep){
//                //mainWindow.Update();
//                timeSinceLastUpdate-=updateTimeStep;
//                //lastUpdateTime=lastUpdateaddMSecs(updateTimeStep);

//        }
//        //mainWindow.render();





//    }


    return app.exec();
}
