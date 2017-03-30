#include "include/GUI.h"
#include "ui_GUI.h"

GUI::GUI(QWidget *parent) :
  QMainWindow(parent),
  m_ui(new Ui::GUI)
{
  m_ui->setupUi(this);
  m_gl = new GLWindow(this);
  m_ui->MainWindow_gridLayout->addWidget(m_gl,0,0,1,1);

  connect(m_ui->m_particleSize,SIGNAL(valueChanged(double)),m_gl,SLOT(setParticleSize(double)));
  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setParticleType(int)));
  connect(m_ui->m_cancel,SIGNAL(pressed()),m_gl,SLOT(cancel()));
  connect(m_ui->m_showConnections,SIGNAL(toggled(bool)),m_gl,SLOT(showConnections(bool)));
  connect(m_ui->m_shadineType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setShading(QString)));
  connect(m_ui->m_LP_forces,SIGNAL(toggled(bool)),m_gl,SLOT(toggleForces(bool)));
  connect(m_ui->m_LP_cohesion,SIGNAL(valueChanged(int)),m_gl,SLOT(setCohesion(int)));
  connect(m_ui->LP_bulge,SIGNAL(released()),m_gl,SLOT(bulge()));
  connect(m_ui->m_LP_spring,SIGNAL(valueChanged(int)),m_gl,SLOT(setSpring(int)));
  connect(m_ui->m_GP_children,SIGNAL(valueChanged(int)),m_gl,SLOT(setChildThreshold(int)));
  connect(m_ui->m_GP_growRadius,SIGNAL(valueChanged(int)),m_gl,SLOT(setGrowthRadius(int)));
  connect(m_ui->m_restart,SIGNAL(pressed()),m_gl,SLOT(restart()));
  connect(m_ui->m_splitType,SIGNAL(currentIndexChanged(QString)),m_gl,SLOT(setSplitType(QString)));
  connect(m_ui->m_GP_branchLenght,SIGNAL(valueChanged(double)),m_gl,SLOT(setBranchLength(double)));
  connect(m_ui->m_particleType,SIGNAL(currentIndexChanged(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetBranchLength(double)),m_ui->m_GP_branchLenght,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetChildrenThreshold(int)),m_ui->m_GP_children,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetCohesion(int)),m_ui->m_LP_cohesion,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(resetForces(bool)),m_ui->m_LP_forces,SLOT(setChecked(bool)));
  connect(m_gl,SIGNAL(resetParticleSize(double)),m_ui->m_particleSize,SLOT(setValue(double)));
  connect(m_gl,SIGNAL(resetParticleTap(int)),m_ui->m_particleTab,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetParticleType(int)),m_ui->m_particleType,SLOT(setCurrentIndex(int)));
  connect(m_gl,SIGNAL(resetSpring(int)),m_ui->m_LP_spring,SLOT(setValue(int)));
  connect(m_gl,SIGNAL(enableGrowthParticle(bool)),m_ui->m_growthParticleTab,SLOT(setEnabled(bool)));
  connect(m_gl,SIGNAL(enableLinkedParticle(bool)),m_ui->m_linkedParticleTab,SLOT(setEnabled(bool)));
}

GUI::~GUI()
{
  delete m_gl;
  delete m_ui;
}
