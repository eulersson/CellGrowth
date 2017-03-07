#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include "Scene.h"

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
  Scene *m_gl;

};

#endif // GUI_H
