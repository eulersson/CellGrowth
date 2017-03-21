#include "include/GUI.h"
#include "ui_GUI.h"

GUI::GUI(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::GUI)
{
  m_ui->setupUi(this);
  m_gl = new GLWindow(this);
  m_ui->MainWindow_gridLayout->addWidget(m_gl,0,0,1,1);


  //Ui connections
//  connect(m_ui->m_particleSize,SIGNAL(valueChanged(double)),m_gl,SLOT(setParticleSize(double)));
//  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setParticleType(int)));
//  connect(m_ui->m_cancle,SIGNAL(pressed()),m_gl,SLOT(cancle()));
//  connect(m_ui->m_showConnections,SIGNAL(toggled(bool)),m_gl,SLOT(showConnections(bool)));
//  connect(m_ui->m_shadineType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setShading(QString)));
//  connect(m_ui->m_LP_forces,SIGNAL(toggled(bool)),m_gl,SLOT(toggleForces(bool)));
//  connect(m_ui->m_LP_cohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setCohesion(int)));
//  connect(m_ui->LP_bulge,SIGNAL(released()),m_gl,SLOT(bulge()));
//  connect(m_ui->m_LP_spring,SIGNAL(valueChanged(int)),m_gl,SLOT(setSpring(int)));
//  connect(m_ui->m_GP_branches,SIGNAL(valueChanged(int)),m_gl,SLOT(setBranchLength(int)));
//  connect(m_ui->m_GP_growRadius,SIGNAL(valueChanged(int)),m_gl,SLOT(setGrowthRadius(int)));
//  connect(m_ui->m_restart,SIGNAL(pressed()),m_gl,SLOT(restart()));
//  connect(m_ui->m_splitType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setSplitType(QString)));

}

GUI::~GUI()
{
  delete m_ui;
}
