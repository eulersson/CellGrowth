#include <iostream>
#include <QtGui/QGuiApplication>
#include "particleswindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    ParticlesWindow window;
    window.show();
    window.setAnimating(true);

    return app.exec();
}
