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

#include <QGuiApplication>

#include "Scene.h"
#include "GUI.h"
int main(int argc, char *argv[])
{



  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setVersion(4,5);
  fmt.setSamples(16);
  fmt.setSwapInterval(1);

  QSurfaceFormat::setDefaultFormat(fmt);
  //window.setFormat(fmt);


  QGuiApplication app(argc, argv);
  GUI window;
  //Scene scene(&window);
  //window.setScene(&scene);

  window.show();

  return app.exec();
}
