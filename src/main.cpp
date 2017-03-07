////////////////////////////////////////////////////////////////////////////////
/// @file main.cpp
/// @author Lola Gille
/// @author Glenn Nygard
/// @author Lydia Kenton
/// @author Esme Prior
/// @author Valeria Marstrom
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

#include <QGuiApplication>

#include "Scene.h"
#include "Window.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  Window window;
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setVersion(4,5);
  fmt.setSamples(16);
  fmt.setSwapInterval(1);
  window.setFormat(fmt);

  Scene scene(&window);
  window.setScene(&scene);

  window.resize(720, 720);
  window.show();

  return app.exec();
}
