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


#include "GUI.h"

int main(int argc, char *argv[])
{

  //double check if need that
  QSurfaceFormat fmt;
  fmt.setProfile(QSurfaceFormat::CoreProfile);
  fmt.setVersion(4,5);
  fmt.setSamples(16);
  fmt.setSwapInterval(1);

  QSurfaceFormat::setDefaultFormat(fmt);
  //window.setFormat(fmt);


  QApplication app(argc, argv);
  GUI window;

  window.show();

  return app.exec();
}
