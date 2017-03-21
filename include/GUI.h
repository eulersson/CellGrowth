#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "GLWindow.h"

////////////////////////////////////////////////////////////////////////////////
/// @file GUI.h
/// @author Carola Gille
/// @version 0.0.1
/// @class GUI
/// @brief Graphic User Interface to interact with the program.
////////////////////////////////////////////////////////////////////////////////

namespace Ui {
class GUI;
}

class GUI : public QMainWindow
{
  Q_OBJECT

public:
  explicit GUI(QWidget *parent = 0);
  ~GUI();

private:
  Ui::GUI *m_ui;

  // Open Gl widget which displays the Cells
  GLWindow *m_gl;

};

#endif // GUI_H
