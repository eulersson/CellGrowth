#include <QtGui/QGuiApplication>
#include "openglwindow.h"

QTime lastUpdateTime = QTime::currentTime();
int updateTimeStep = 5;

int main(int argc, char **argv) {
    //QGuiApplication app(argc, argv);
    /*QCoreApplication is the base class, QGuiApplication extends the base class
     * with functionality related to handling windows and GUI stuff (non-widget related, e.g.
     * OpenGL or QtQuick), QApplication extends QGuiApplication with functionality
     * related to handling widgets.*/

    QApplication app(argc, argv);




    // Create and display window
    OpenGLWindow mainWindow;
    mainWindow.show();
    mainWindow.setAnimating(true); // Render function will be continously called
    //mainWindow.setInputManager(&input);




    return app.exec();
}
