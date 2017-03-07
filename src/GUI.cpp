#include "include/GUI.h"
#include "ui_GUI.h"

GUI::GUI(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::GUI)
{
  m_ui->setupUi(this);
  m_gl = new GLWindow(this);
//  m_ui->MainWindow_gridLayout->addWidget(m_gl,0,0,1,1);

}

GUI::~GUI()
{
  delete m_ui;
}
