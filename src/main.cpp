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

#include "GLWindow.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  GLWindow window;

  window.resize(720, 720);
  window.show();

  return app.exec();
}
