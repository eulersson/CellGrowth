
////////////////////////////////////////////////////////////////////////////////
/// @file main.cpp
/// @author Carola Gille
/// @author Glenn Nygard
/// @author Lydia Kenton
/// @author Esme Prior
/// @author Valeria Marstrom
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include <QApplication>

#include <QtGlobal>
#include "GUI.h"

int main(int argc, char *argv[])
{

    #ifdef Q_OS_MACX
    QSurfaceFormat format;
    format.setDepthBufferSize(32);
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    #endif



  QApplication app(argc, argv);
  GUI window;

  window.show();

  return app.exec();
}
